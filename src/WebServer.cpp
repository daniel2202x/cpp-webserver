#include "WebServer.h"

#include <iostream>
#include <iomanip>
#include <ctime>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

constexpr uint32_t RESPONSE_BUFFER_SIZE = 1024;

WebServer::WebServer(uint32_t port)
{
    m_ServerSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServerSocketID < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        std::exit(1);
    }

    sockaddr_in serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(m_ServerSocketID, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        std::exit(1);
    }

    if (listen(m_ServerSocketID, 5) < 0)
    {
        std::cerr << "Error starting listener" << std::endl;
        std::exit(1);
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
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }

        char buffer[RESPONSE_BUFFER_SIZE] = {0};
        ssize_t bytesRead = recv(clientSocketID, buffer, RESPONSE_BUFFER_SIZE - 1, 0);
        if (bytesRead < 0)
        {
            std::cerr << "Error reading request from client" << std::endl;
            close(clientSocketID);
            continue;
        }

        Request request(buffer);
        if (request.IsFaulty())
        {
            std::cerr << "Request is faulty, see output above" << std::endl;
            close(clientSocketID);
            continue;
        }

        const HandlerMap *map = nullptr;
        const std::string &method = request.GetMethod();
        const std::string &url = request.GetUrl();

        if (method == "GET")
            map = &m_GetHandlers;
        else if (method == "POST")
            map = &m_PostHandlers;
        else if (method == "PUT")
            map = &m_PutHandlers;

        if (!map || !map->contains(url))
        {
            std::cerr << "Request " << method << " " << url << " could not be mapped" << std::endl;
            close(clientSocketID);
            continue;
        }

        Response response = map->at(url)(request);

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

        std::stringstream ss;
        ss << "HTTP/1.1 " << response.StatusCode << " " << reason << "\r\n";
        ss << "Content-Type: " << response.ContentType << "\r\n";
        ss << "Content-Length: " << response.Body.size() << "\r\n";
        ss << "Date: " << std::put_time(timeInfo, "%a, %d %b %Y %H:%M:%S GMT") << "\r\n";
        ss << "Cache-Control: no-cache, no-store, max-age=0, must-revalidate\r\n";
        ss << "X-Powered-By: d2x/1.0\r\n";
        ss << "\r\n";
        ss << response.Body;

        std::string rawResponse = ss.str();
        ssize_t bytesSent = send(clientSocketID, rawResponse.c_str(), rawResponse.length(), 0);
        if (bytesSent < 0)
        {
            std::cerr << "Error sending response to client" << std::endl;
            close(clientSocketID);
            continue;
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
