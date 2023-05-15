#pragma once

#include "Request.h"
#include "Response.h"

namespace Logic
{
    Response CreateRoom(const Request &request);
    Response GetRooms(const Request &request);
};
