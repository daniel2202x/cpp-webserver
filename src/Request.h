#pragma once

#include <string>
#include <unordered_map>

class Request
{
public:
    Request(const std::string &rawRequest);

    inline bool IsFaulty() const { return m_IsFaulty; }

private:
    bool m_IsFaulty = false;

    std::string m_Method;
    std::string m_Path;
    std::string m_Protocol;

    std::unordered_map<std::string, std::string> m_Headers;

    std::string m_Body;
};
