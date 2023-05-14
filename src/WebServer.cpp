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

std::optional<Request> WebServer::AwaitNextRequest()
{
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    m_CurrentClientSocketID = accept(m_ServerSocketID, (sockaddr *)&clientAddress, &clientAddressSize);
    if (m_CurrentClientSocketID < 0)
    {
        std::cerr << "Error accepting client connection" << std::endl;
        return {};
    }

    char buffer[RESPONSE_BUFFER_SIZE] = {0};
    int bytesRead = recv(m_CurrentClientSocketID, buffer, RESPONSE_BUFFER_SIZE - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "Error reading request from client" << std::endl;
        return {};
    }

    return Request(buffer);
}

void WebServer::CloseCurrentRequest(const std::string &response)
{
    int bytesSent = send(m_CurrentClientSocketID, response.c_str(), response.length(), 0);
    if (bytesSent < 0)
    {
        std::cerr << "Error sending response to client" << std::endl;
        return;
    }

    close(m_CurrentClientSocketID);
}
