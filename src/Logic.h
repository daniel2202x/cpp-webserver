#pragma once

#include "WebServer.h"

namespace Logic
{
    WebServer::Response CreateRoom(const WebServer::Headers &headers, const std::string &body);
    WebServer::Response GetRooms(const WebServer::Headers &headers, const std::string &body);
};
