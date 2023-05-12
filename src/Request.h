#pragma once

#include <string>

#include "Message.h"

class Request : public Message
{
private:
    std::string m_Method;
    std::string m_Path;
    std::string m_Protocol;

public:
    Request(const std::string &rawRequest);
};
