#pragma once

#include "WebServer.h"

namespace Logic
{
    WebServer::Response CreateRoom(const Request &request);
    WebServer::Response GetRooms(const Request &request);
};
