#pragma once

#include <utility>
#include <optional>
#include <functional>

#include "Request.h"

class WebServer
{
public:
    WebServer(uint32_t port);
    ~WebServer();

    void Run();

    struct Response
    {
        uint16_t StatusCode;
        std::string ContentType;
        std::string Body;
    };

    using Handler = std::function<Response(const Request &)>;
    using HandlerMap = std::unordered_map<std::string, Handler>;

    void Get(const std::string &url, WebServer::Handler handler);
    void Post(const std::string &url, WebServer::Handler handler);
    void Put(const std::string &url, WebServer::Handler handler);

private:
    int32_t m_ServerSocketID;

    HandlerMap m_GetHandlers;
    HandlerMap m_PostHandlers;
    HandlerMap m_PutHandlers;
};
