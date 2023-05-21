#pragma once

#include <utility>
#include <optional>
#include <functional>

class WebServer
{
public:
    WebServer(uint16_t port);
    ~WebServer();

    void Run();

    struct Response
    {
        uint16_t StatusCode;
        std::string ContentType;
        std::string Body;
    };

    using Headers = std::unordered_map<std::string, std::string>;
    using Handler = std::function<Response(const Headers &, const std::string &)>;
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
