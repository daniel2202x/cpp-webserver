#include "Request.h"

#include <iostream>
#include <sstream>

#include "Utility.h"

Request::Request(const std::string &rawRequest)
    : m_IsFaulty(false)
{
    std::istringstream iss(rawRequest);
    std::string line;

    if (!std::getline(iss, line))
    {
        std::cerr << "Request must contain at least the request line" << std::endl;
        m_IsFaulty = true;
        return;
    }

    // TODO
    std::vector<std::string> data = Utility::SplitString(line, ' ');

    if (data.size() != 3)
    {
        std::cerr << "Invalid first request line: " << line << std::endl;
        m_IsFaulty = true;
        return;
    }

    m_Method = data[0];
    m_Url = data[1];
    // m_Protocol = data[2]; no need to store this because nobody cares about the protocol version

    bool hasParsedHeaders = false;
    while (!hasParsedHeaders && std::getline(iss, line))
    {
        // TODO
        line = Utility::TrimString(line);
        if (line != "")
        {
            size_t colonIndex = line.find(":");
            if (colonIndex != std::string::npos)
            {
                // TODO
                std::string headerName = line.substr(0, colonIndex);
                // TODO
                std::string headerValue = line.substr(colonIndex + 1, line.size() - 1);
                // TODO
                m_Headers[Utility::ToLowerCase(headerName)] = Utility::TrimString(headerValue);
            }
            else
            {
                std::cerr << "Invalid header: " << line << std::endl;
                m_IsFaulty = true;
                return;
            }
        }
        else
        {
            hasParsedHeaders = true;
        }
    }

    std::stringstream bodyStream;
    while (std::getline(iss, line))
    {
        bodyStream << line << "\n";
    }
    m_Body = bodyStream.str();
}
