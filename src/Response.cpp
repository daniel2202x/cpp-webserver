#include "Response.h"

#include <sstream>

Response::Response(uint32_t code, const std::string &contentType, const std::string &body)
{
    std::stringstream ss;
    ss << "HTTP/1.1 " << code << " \r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "Cache-Control: no-cache\r\n";
    ss << "\r\n";
    ss << body;
    m_Raw = ss.str();
}
