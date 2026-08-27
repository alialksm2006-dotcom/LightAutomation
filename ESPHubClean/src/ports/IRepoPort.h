#pragma once
#include <vector>
#include "domain/entities/Light.h"
#include "ports/ILightConnectionData.h"
#include "ports/ILightExecuteConnection.h"  
class IRepoPort
{
public:
  struct SmartDevice {
    Light lightEntity;
    ILightConnectionData *lightConnectionData;
    ILightExecuteConnection* executeConnection;
  
 };
    virtual std::vector<SmartDevice> &getDevices() = 0;
    virtual SmartDevice *getDeviceById(int id) = 0;
    virtual void addDevice(SmartDevice newDevice) = 0;
    virtual bool deleteDeviceById(uint32_t id) = 0;

};