#include "WebServer.h"
#include "Logic.h"

static constexpr uint16_t DEFAULT_PORT = 1234;

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc == 2)
    {
        // port = argv[1];
    }

    WebServer server(port);

    server.Get("/rooms", Logic::GetRooms);

    server.Run();

    return 0;
}