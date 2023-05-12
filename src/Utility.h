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
}