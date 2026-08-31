#pragma once
#include "ControllerSource.h"
class ButtonSource : public ControllerSource
{


public:
    enum class Type
    {
        push = 0,
        switch_ = 1
    };

    private:
Type type;
};