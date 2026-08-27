#pragma once 
#include <vector> 
#include "ports/ISwitch.h"
class IRepoSwitch
{
    public :
    virtual std::vector<ISwitch*> &getSwitches() = 0;
    virtual void addSwitch(int pinNum,int devId) = 0;
    virtual void deleteSwitch(int pin) = 0;
    virtual ISwitch* getSwitchByDeviceId(int devId)=0;
    virtual ~IRepoSwitch() = default;
};