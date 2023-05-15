#pragma once

#include <utility>
#include <optional>

#include "Request.h"
#include "Response.h"

constexpr int RESPONSE_BUFFER_SIZE = 1024;

using Handler = Response (*)(const Request &);
using HandlerMap = std::unordered_map<std::string, Handler>;

class WebServer
{
public:
    WebServer(uint32_t port);
    ~WebServer();

    void Run();

    void Get(const std::string &url, Handler handler);
    void Post(const std::string &url, Handler handler);
    void Put(const std::string &url, Handler handler);

private:
    void AwaitNextRequest();
    void CloseCurrentRequest(const Response &response);

    int32_t m_ServerSocketID;
    int32_t m_CurrentClientSocketID;

    std::optional<Request> m_CurrentRequest;

    HandlerMap m_GetHandlers;
    HandlerMap m_PostHandlers;
    HandlerMap m_PutHandlers;
};
