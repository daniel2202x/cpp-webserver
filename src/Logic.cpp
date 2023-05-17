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

bool ValidatePostPutHeaders(const WebServer::Headers &headers)
{
    if (headers.contains("accept"))
    {
        const std::string &acceptHeader = headers.at("accept");
        if (acceptHeader.find("text/plain") != std::string::npos || acceptHeader.find("*/*") != std::string::npos)
        {
            return true;
        }
        else
        {
            std::cerr << "Accept header doesn't contain 'text/plain'" << std::endl;
        }
    }
    else
    {
        std::cerr << "Accept header missing" << std::endl;
    }

    return false;
}

WebServer::Response Logic::CreateRoom(const WebServer::Headers &headers, const std::string &body)
{
    VALIDATE_POST_PUT_HEADERS(headers)

    s_Rooms.emplace_back(body);

    return {201, "text/plain", ""};
}

WebServer::Response Logic::GetRooms(const WebServer::Headers &headers, const std::string &body)
{
    return {200, "text/plain", "Room 1\nRoom 2"};
}
