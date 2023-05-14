#pragma once

#include <string>

class Response
{
public:
    Response(uint32_t code, const std::string &contentType, const std::string &body);

    inline const std::string &GetRaw() const { return m_Raw; }

private:
    std::string m_Raw;
};
