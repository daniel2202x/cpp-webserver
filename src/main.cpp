#include <iostream>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "WebServer.h"

constexpr int DEFAULT_PORT = 1234;

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc == 2)
    {
        // port = argv[1];
    }

    WebServer server(port);

    while (true)
    {
        if (server.AcceptNextRequest())
        {
            auto request = server.OpenNextRequest();
            if (request.has_value())
            {
                std::cout << "Recieved request: " << request.value() << std::endl;

                std::string response = R"(HTTP/1.1 200 OK
Content-Type: text/plain

asdf)";

                server.CloseCurrentRequest(response);
            }
        }
    }

    return 0;
}