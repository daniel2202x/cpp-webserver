#pragma once

#include <utility>
#include <string>
#include <optional>

#include "Request.h"

constexpr int RESPONSE_BUFFER_SIZE = 1024;

class WebServer
{
private:
    int32_t m_ServerSocketID;
    int32_t m_CurrentClientSocketID;

public:
    WebServer(uint32_t port);
    ~WebServer();

    bool AcceptNextRequest();
    std::optional<Request> OpenNextRequest();
    void CloseCurrentRequest(const std::string &response);
};
