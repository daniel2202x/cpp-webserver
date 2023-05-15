#include "Logic.h"

#include <iostream>
#include <sstream>
#include <vector>

#define VALIDATE_POST_PUT_HEADERS(request)      \
    if (!ValidatePostPutHeaders(request))       \
    {                                           \
        return Response(400, "text/plain", ""); \
    }

static std::vector<std::string> s_Rooms;

bool ValidatePostPutHeaders(const Request &request)
{
    if (request.GetHeaders().contains("accept"))
    {
        const std::string &acceptHeader = request.GetHeaders().at("accept");
        if (acceptHeader.find("text/plain") != std::string::npos || acceptHeader.find("*/*") != std::string::npos)
        {
            return true;
        }
        else
        {
            std::cerr << "Accept header doesn't contain 'text/plain' on request " << request.GetMethod() << " " << request.GetPath() << std::endl;
        }
    }
    else
    {
        std::cerr << "Accept header missing on request " << request.GetMethod() << " " << request.GetPath() << std::endl;
    }

    return false;
}

Response Logic::CreateRoom(const Request &request)
{
    VALIDATE_POST_PUT_HEADERS(request)

    s_Rooms.emplace_back(request.GetBody());

    return Response(201, "text/plain", "");
}

Response Logic::GetRooms(const Request &request)
{
    return Response(200, "text/plain", "Room 1\nRoom 2");
}
