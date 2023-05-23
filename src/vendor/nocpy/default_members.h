#define DEFAULT_MEMBERS(type)               \
    type() : std::type()                    \
    {                                       \
    }                                       \
                                            \
    type(const string &) = delete;          \
    type &operator=(const type &) = delete; \
                                            \
    type(type &&other) : std::type(other)   \
    {                                       \
    }                                       \
    type &operator=(type &&other)           \
    {                                       \
        std::type::operator=(other);        \
        return *this;                       \
    }
