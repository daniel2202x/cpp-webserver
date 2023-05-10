#include <iostream>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

constexpr int DEFAULT_PORT = 1234;
constexpr int RESPONSE_BUFFER_SIZE = 1024;

void handleRequest(int clientSocket)
{
    char buffer[RESPONSE_BUFFER_SIZE] = {0};
    int bytesRead = recv(clientSocket, buffer, RESPONSE_BUFFER_SIZE - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "Error reading request from client" << std::endl;
        return;
    }

    std::string request(buffer);

    std::cout << "Recieved request: " << request << std::endl;

    std::string response = "Response: " + request;

    int bytesSent = send(clientSocket, response.c_str(), response.length(), 0);
    if (bytesSent < 0)
    {
        std::cerr << "Error reading request from client" << std::endl;
        return;
    }

    close(clientSocket);
}

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc == 2)
    {
        // port = argv[1];
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Error starting listener" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressSize);
        if (clientSocket < 0)
        {
            std::cerr << "Error accepting client connection" << std::endl;
            return 1;
        }

        handleRequest(clientSocket);
    }

    close(serverSocket);

    return 0;
}