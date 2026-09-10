#pragma once
#include "ControllerSource.h"
class ButtonSource : public ControllerSource
{

public:
    enum class Type
    {
        PUSH = 0,
        SWITCH = 1
    };

private:
    Type type;

public:
    Type getType() const
    {
        return type;
    }

    void setType(Type type)
    {
        this->type = type;
    }
    std::string getDetails() const override
    {
        switch (type)
        {
        case Type::PUSH:
            return "PUSH";
        case Type::SWITCH:
            return "SWITCH";
        default:
            return "UNKNOWN";
        }
    }
};