#pragma once

#include <string>
#include <unordered_map>

class Request
{
public:
    Request(const std::string &rawRequest);

    inline bool IsFaulty() const { return m_IsFaulty; }

    inline const std::string &GetMethod() const { return m_Method; }
    inline const std::string &GetUrl() const { return m_Url; }

    inline const std::unordered_map<std::string, std::string> &GetHeaders() const { return m_Headers; }

    inline const std::string &GetBody() const { return m_Body; }

private:
    bool m_IsFaulty;

    std::string m_Method;
    std::string m_Url;

    std::unordered_map<std::string, std::string> m_Headers;

    std::string m_Body;
};
