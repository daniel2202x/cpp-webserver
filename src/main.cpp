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
        std::optional<Request> request = server.AwaitNextRequest();
        if (request.has_value())
        {
            Response response(200, "text/plain", "asdf");
            server.CloseCurrentRequest(response);
        }
    }

    return 0;
}