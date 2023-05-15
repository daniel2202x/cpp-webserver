#include "Logic.h"

#include <sstream>

Response Logic::CreateRoom(const Request &request)
{
    // validate Content-Type and Accept headers

    std::istringstream iss(request.GetBody());

    return Response(200, "text/plain", "");
}

Response Logic::GetRooms(const Request &request)
{
    // validate Content-Type and Accept headers

    return Response(200, "text/plain", "Room 1\nRoom 2");
}
