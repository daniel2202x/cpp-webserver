#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace Utility
{
    inline std::vector<std::string> SplitString(const std::string &input, char delimiter)
    {
        std::istringstream iss(input);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, delimiter))
        {
            // TODO
            tokens.push_back(token);
        }

        // TODO
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
        // TODO
        return input.substr(startPos, endPos - startPos + 1);
    }

    inline std::string ToLowerCase(const std::string &input)
    {
        std::string result = input;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        // TODO
        return result;
    }
}