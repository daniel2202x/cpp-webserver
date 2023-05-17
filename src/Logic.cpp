#include "Logic.h"

#include <iostream>
#include <sstream>
#include <vector>

#define VALIDATE_POST_PUT_HEADERS(request)                                                            \
    if (!ValidatePostPutHeaders(request))                                                             \
    {                                                                                                 \
        return {400, "text/plain", "No Accept header with the value of 'text/plain' was specified."}; \
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
            std::cerr << "Accept header doesn't contain 'text/plain' on request " << request.GetMethod() << " " << request.GetUrl() << std::endl;
        }
    }
    else
    {
        std::cerr << "Accept header missing on request " << request.GetMethod() << " " << request.GetUrl() << std::endl;
    }

    return false;
}

WebServer::Response Logic::CreateRoom(const Request &request)
{
    VALIDATE_POST_PUT_HEADERS(request)

    s_Rooms.emplace_back(request.GetBody());

    return {201, "text/plain", ""};
}

WebServer::Response Logic::GetRooms(const Request &request)
{
    return {200, "text/plain", "Room 1\nRoom 2"};
}
