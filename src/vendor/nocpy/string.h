#include <string>

#include "default_members.h"

namespace nocpy
{
    class string : public std::string
    {
    public:
        DEFAULT_MEMBERS(string)

        string(const char *cStr) : std::string(cStr)
        {
        }
    };
}
