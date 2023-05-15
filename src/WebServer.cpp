#include "WebServer.h"

#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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
        AwaitNextRequest();
        if (m_CurrentRequest.has_value())
        {
            const HandlerMap *map = nullptr;
            const std::string &method = m_CurrentRequest->GetMethod();

            if (method == "GET")
                map = &m_GetHandlers;
            else if (method == "POST")
                map = &m_PostHandlers;
            else if (method == "PUT")
                map = &m_PutHandlers;

            const std::string &url = m_CurrentRequest->GetPath();
            if (map && map->contains(url))
            {
                Response response = map->at(url)(m_CurrentRequest.value());
                CloseCurrentRequest(response);
            }
            else
            {
                CloseCurrentRequest(Response(404, "text/plain", "Resource not found"));
            }
        }
    }
}

void WebServer::Get(const std::string &url, Handler handler)
{
    m_GetHandlers[url] = handler;
}

void WebServer::Post(const std::string &url, Handler handler)
{
    m_PostHandlers[url] = handler;
}

void WebServer::Put(const std::string &url, Handler handler)
{
    m_PutHandlers[url] = handler;
}

void WebServer::AwaitNextRequest()
{
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    m_CurrentClientSocketID = accept(m_ServerSocketID, (sockaddr *)&clientAddress, &clientAddressSize);
    if (m_CurrentClientSocketID < 0)
    {
        std::cerr << "Error accepting client connection" << std::endl;
        m_CurrentRequest = {};
        return;
    }

    char buffer[RESPONSE_BUFFER_SIZE] = {0};
    int bytesRead = recv(m_CurrentClientSocketID, buffer, RESPONSE_BUFFER_SIZE - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "Error reading request from client" << std::endl;
        m_CurrentRequest = {};
        return;
    }

    Request request(buffer);
    if (request.IsFaulty())
    {
        std::cerr << "Request is faulty, see output above" << std::endl;
        CloseCurrentRequest(Response(400, "text/plain", "Request is faulty"));
        m_CurrentRequest = {};
        return;
    }

    m_CurrentRequest = request;
}

void WebServer::CloseCurrentRequest(const Response &response)
{
    const std::string &rawResponse = response.GetRaw();
    int bytesSent = send(m_CurrentClientSocketID, rawResponse.c_str(), rawResponse.length(), 0);
    if (bytesSent < 0)
    {
        std::cerr << "Error sending response to client" << std::endl;
        return;
    }

    close(m_CurrentClientSocketID);
}
