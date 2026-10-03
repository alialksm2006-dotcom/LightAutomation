#pragma once
#include "ControllerSource.h"
class ButtonSource 
{

public:
    enum class Type
    {
        PUSH = 0,
        SWITCH = 1
    };

private:
    int id = 0;
    Type type = Type::PUSH;
    int controllerSourceId = 0;

public:
    int getId() const
    {
        return id;
    }

    void setId(int id)
    {
        this->id = id;
    }

    Type getType() const
    {
        return type;
    }

    void setType(Type type)
    {
        this->type = type;
    }

    int getControllerSourceId() const
    {
        return controllerSourceId;
    }

    void setControllerSourceId(int controllerSourceId)
    {
        this->controllerSourceId = controllerSourceId;
    }
    std::string getDetails() const 
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