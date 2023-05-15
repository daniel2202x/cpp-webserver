#include "Response.h"

#include <sstream>
#include <iomanip>
#include <ctime>

Response::Response(uint32_t code, const std::string &contentType, const std::string &body)
{
    const char *reason = "";
    switch (code)
    {
    case 200:
        reason = "OK";
        break;
    case 201:
        reason = "Created";
        break;
    case 400:
        reason = "Bad Request";
        break;
    }

    std::time_t currentTime = std::time(nullptr);
    std::tm *timeInfo = std::gmtime(&currentTime);

    std::stringstream ss;
    ss << "HTTP/1.1 " << code << " " << reason << "\r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "Date: " << std::put_time(timeInfo, "%a, %d %b %Y %H:%M:%S GMT") << "\r\n";
    ss << "Cache-Control: no-cache, no-store, max-age=0, must-revalidate\r\n";
    ss << "X-Powered-By: d2x/1.0\r\n";
    ss << "\r\n";
    ss << body;
    m_Raw = ss.str();
}
