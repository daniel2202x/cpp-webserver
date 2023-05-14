#pragma once

#include <utility>
#include <optional>

#include "Request.h"
#include "Response.h"

constexpr int RESPONSE_BUFFER_SIZE = 1024;

class WebServer
{
public:
    WebServer(uint32_t port);
    ~WebServer();

    std::optional<Request> AwaitNextRequest();
    void CloseCurrentRequest(const Response &response);

private:
    int32_t m_ServerSocketID;
    int32_t m_CurrentClientSocketID;
};
