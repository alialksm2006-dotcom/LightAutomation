#pragma once
#include "ports/IRepoSwitch.h"
#include <vector>
#include "adapters/gpio_input/RockerSwitch.h"
#include <Arduino.h>
#include "usecases/ToggleLightUseCase.h"
class SwitchesRepo :public IRepoSwitch
{
    private :
   static std::vector<ISwitch*> switches;
    public :

    void init()
    {
        for(ISwitch *s : switches)
        {
            if(s!=nullptr)
            s->setup();
        }
    }

    void update(ToggleLightUseCase toggle)
    {
        for(ISwitch *s : switches)
        {
            if(s!=nullptr)
            s->update( toggle);
        }
    }
    std::vector<ISwitch*> &getSwitches() override
    {
        return switches;
    }

    RockerSwitch* getSwitchByDeviceId(int devId)override
    {
        for (ISwitch* sw : switches)
        {
            RockerSwitch* rocker = dynamic_cast<RockerSwitch*>(sw);
            if(rocker) Serial.println("D Id is : "+String(rocker->getDeviceId()));
            if (rocker && rocker->getDeviceId() == devId)
            {
                return rocker;
            }
        }
        return nullptr; 
    } 
    void addSwitch(int pinNum, int devId) override
    {
        ISwitch* newSwitch = new RockerSwitch(pinNum, devId);
        newSwitch->setup();
        switches.push_back(newSwitch);
    }
    void deleteSwitch(int pin) override
    {
        auto it = std::find_if(switches.begin(), switches.end(),
                                 [pin](ISwitch*& sw)
                                 {
                                     RockerSwitch* rocker = dynamic_cast<RockerSwitch*>(sw);
                                     return rocker && rocker->getPin() == pin&&rocker->getType()=="RockerSwitch /Main Controller";
                                 });

        if (it != switches.end())
        {
            delete *it; 
          switches.erase(it);
        }
    
    }
};