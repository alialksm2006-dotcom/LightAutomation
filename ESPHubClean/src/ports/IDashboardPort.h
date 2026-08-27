#pragma once 
#include "domain/entities/Light.h"
class IDashboardPort {
    public :
    virtual void updateLightState(const Light& light)=0;
    virtual std::string getName()=0;
};