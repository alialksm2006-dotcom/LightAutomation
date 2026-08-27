#pragma once
#include <vector>
#include <Arduino.h>
#include <algorithm>
#include "ports/IRepoPort.h"
#include "ports/ILightConnectionData.h"
class LightsRepo : public IRepoPort
{
private:
    static std::vector<SmartDevice> smartDevices;

public:
    static void init()
    {
    }

    std::vector<IRepoPort::SmartDevice> &getDevices() override
    {
        return smartDevices;
    }
    LightsRepo::SmartDevice *getDeviceById(int id) override
    {
        
             Serial.println("Devices id is : ");

           for(int i=0;i<smartDevices.size();i++)
           {
            Serial.println(smartDevices[i].lightEntity.id);
            if(smartDevices[i].lightEntity.id==id)

{            return &smartDevices[i];
    Serial.println("Device was Found ");
}   
        }
        
        Serial.println("Device was not found");
        return nullptr;
    }

    void addDevice(IRepoPort::SmartDevice newDevice) override
    {
        smartDevices.push_back(newDevice);
    }
    bool deleteDeviceById(uint32_t id) override
    {
        Serial.println("Deleting device with ID: " + String(id));

        auto it = std::find_if(smartDevices.begin(), smartDevices.end(),
                               [&](const SmartDevice &d)
                               {
                                   return d.lightEntity.id == id;
                               });

        if (it == smartDevices.end())
        {
            return false;
        }

        delete it->lightConnectionData;  // 🔥 احذف أولاً

        smartDevices.erase(it); // ثم احذف من vector
        return true;
    }
};
