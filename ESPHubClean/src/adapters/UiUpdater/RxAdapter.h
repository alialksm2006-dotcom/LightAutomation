#pragma once 
#include "ports/IDashboardPort.h"
#include "domain/entities/Light.h"
#include "adapters/MyRemoteXY.h"
class RxAdapter :public IDashboardPort
{   MyRemoteXY *remoteXY;
    public:
    RxAdapter(MyRemoteXY *remoteXY):remoteXY(remoteXY){}
     void updateLightState(const Light& light) override
    {
      remoteXY->updateDashboard(light.id,light.getState());
      TelnetStream.println("update RX");
    }
    std::string getName()override;
    
};