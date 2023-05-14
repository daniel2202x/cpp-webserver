#pragma once

#include <string>
#include <unordered_map>

class Message
{
public:
    inline bool IsFaulty() const { return m_IsFaulty; }

protected:
    bool m_IsFaulty = false;

    std::unordered_map<std::string, std::string> m_Headers;
    std::string m_Body;

    Message() = default;
    virtual ~Message() = default;
};
