#include "WebServer.h"

#include <iostream>
#include <iomanip>
#include <ctime>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LOG_AND_EXIT(error)          \
    std::cerr << error << std::endl; \
    std::exit(1)

#define LOG_AND_CONTINUE(error)      \
    std::cerr << error << std::endl; \
    continue

#define LOG_CLOSE_AND_CONTINUE(error) \
    std::cerr << error << std::endl;  \
    close(clientSocketID);            \
    continue

static constexpr uint32_t RESPONSE_BUFFER_SIZE = 1024;

WebServer::WebServer(uint16_t port)
{
    m_ServerSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServerSocketID < 0)
    {
        LOG_AND_EXIT("Error creating socket");
    }

    sockaddr_in serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(m_ServerSocketID, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        LOG_AND_EXIT("Error binding socket");
    }

    if (listen(m_ServerSocketID, 5) < 0)
    {
        LOG_AND_EXIT("Error starting listener");
    }

    std::cout << "Server listening on port " << port << std::endl;
}

WebServer::~WebServer()
{
    close(m_ServerSocketID);
}

void WebServer::Run()
{
    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int32_t clientSocketID = accept(m_ServerSocketID, (sockaddr *)&clientAddress, &clientAddressSize);
        if (clientSocketID < 0)
        {
            LOG_AND_CONTINUE("Error accepting client connection");
        }

        char requestBuffer[RESPONSE_BUFFER_SIZE] = {0};
        ssize_t bytesRead = recv(clientSocketID, requestBuffer, RESPONSE_BUFFER_SIZE - 1, 0);
        if (bytesRead < 0)
        {
            LOG_CLOSE_AND_CONTINUE("Error reading request from client");
        }

        std::istringstream requestStream(requestBuffer);
        std::string requestStreamLine;

        // read the request title line
        if (!std::getline(requestStream, requestStreamLine))
        {
            LOG_CLOSE_AND_CONTINUE("Request must contain at least the request line");
        }

        std::istringstream requestTitleStream(requestStreamLine);
        nocpy::string requestTitleStreamLine;
        std::getline(requestTitleStream, requestTitleStreamLine, ' ');
        nocpy::string method = std::move(requestTitleStreamLine);
        std::getline(requestTitleStream, requestTitleStreamLine, ' ');
        nocpy::string url = std::move(requestTitleStreamLine);

        // read the request headers
        Headers headers;
        bool doneParsingHeaders = false;
        while (!doneParsingHeaders && std::getline(requestStream, requestStreamLine))
        {
            size_t colonIndex = requestStreamLine.find(": ");
            if (colonIndex != std::string::npos)
            {
                std::string headerName = requestStreamLine.substr(0, colonIndex);
                std::transform(headerName.begin(), headerName.end(), headerName.begin(),
                               [](uint8_t c)
                               { return std::tolower(c); });
                headers[headerName] = requestStreamLine.substr(colonIndex + 2, requestStreamLine.size() - headerName.size() - 3);
            }
            else
            {
                doneParsingHeaders = true;
            }
        }

        // read the request body
        std::stringstream bodyStream;
        while (std::getline(requestStream, requestStreamLine))
        {
            bodyStream << requestStreamLine << "\n";
        }
        std::string body = bodyStream.str();

        const HandlerMap *map = nullptr;

        if (method == "GET")
            map = &m_GetHandlers;
        else if (method == "POST")
            map = &m_PostHandlers;
        else if (method == "PUT")
            map = &m_PutHandlers;

        if (!map || !map->contains(url))
        {
            LOG_CLOSE_AND_CONTINUE("Request " << method << " " << url << " could not be mapped");
        }

        Response response = map->at(url)(headers, body);

        const char *reason = "";
        switch (response.StatusCode)
        {
        case 200:
            reason = "OK";
            break;
        case 201:
            reason = "Created";
            break;
        case 400:
            reason = "Bad Request";
            break;
        case 401:
            reason = "Unauthorised";
            break;
        case 403:
            reason = "Forbidden";
            break;
        case 404:
            reason = "Not Found";
            break;
        case 500:
            reason = "Internal Server Error";
            break;
        }

        std::time_t currentTime = std::time(nullptr);
        std::tm *timeInfo = std::gmtime(&currentTime);

        std::stringstream responseStream;
        responseStream << "HTTP/1.1 " << response.StatusCode << " " << reason << "\r\n";
        responseStream << "Content-Type: " << response.ContentType << "\r\n";
        responseStream << "Content-Length: " << response.Body.size() << "\r\n";
        responseStream << "Date: " << std::put_time(timeInfo, "%a, %d %b %Y %H:%M:%S GMT") << "\r\n";
        responseStream << "Cache-Control: no-cache, no-store, max-age=0, must-revalidate\r\n";
        responseStream << "X-Powered-By: d2x/1.0\r\n";
        responseStream << "\r\n";
        responseStream << response.Body;

        std::string rawResponse = responseStream.str();
        ssize_t bytesSent = send(clientSocketID, rawResponse.c_str(), rawResponse.length(), 0);
        if (bytesSent < 0)
        {
            LOG_CLOSE_AND_CONTINUE("Error sending response to client for request " << method << " " << url);
        }

        close(clientSocketID);
    }
}

void WebServer::Get(const std::string &url, WebServer::Handler handler)
{
    m_GetHandlers[url] = handler;
}

void WebServer::Post(const std::string &url, WebServer::Handler handler)
{
    m_PostHandlers[url] = handler;
}

void WebServer::Put(const std::string &url, WebServer::Handler handler)
{
    m_PutHandlers[url] = handler;
}
