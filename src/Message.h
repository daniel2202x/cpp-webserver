#pragma once

class Message
{
protected:
    bool m_IsFaulty = false;

    Message() = default;
    virtual ~Message() = default;

public:
    inline bool IsFaulty() const { return m_IsFaulty; }
};
