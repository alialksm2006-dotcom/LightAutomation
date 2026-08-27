#pragma once 
#include "ports/IOutputPort.h"
class EspNowAdapter:public IOutputPort
{
    public: 
    void execute(const Light &light) override
    {

    }
};