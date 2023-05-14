#include <string>
#include <vector>
#include <sstream>

namespace Utility
{
    inline std::vector<std::string> SplitString(const std::string &value, char delimiter)
    {
        std::istringstream iss(value);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }

    inline std::string TrimString(const std::string &input)
    {
        std::size_t startPos = input.find_first_not_of(" \t\r\n");
        if (startPos == std::string::npos)
        {
            return "";
        }

        std::size_t endPos = input.find_last_not_of(" \t\r\n");
        return input.substr(startPos, endPos - startPos + 1);
    }
}