#include "Request.h"

#include <iostream>
#include <sstream>

#include "Utility.h"

Request::Request(const std::string &rawRequest)
{
    std::istringstream iss(rawRequest);
    std::string line;

    if (!std::getline(iss, line))
    {
        std::cerr << "Request must contain at least one line" << std::endl;
        m_IsFaulty = true;
        return;
    }

    std::vector<std::string> data = Utility::SplitString(line, ' ');

    if (data.size() != 3)
    {
        std::cerr << "Invalid request line" << std::endl;
        m_IsFaulty = true;
        return;
    }

    m_Method = data[0];
    m_Path = data[1];
    m_Protocol = data[2];
}
