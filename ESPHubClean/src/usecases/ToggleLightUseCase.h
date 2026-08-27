#pragma once 
#include "ports/IRepoPort.h"
#include "ports/IDashboardPort.h"
#include <vector>
class ToggleLightUseCase {
private:
    IRepoPort* repo;
    std::vector<IDashboardPort*> &dashboards;
    
public:
    ToggleLightUseCase(IRepoPort* repo,std::vector<IDashboardPort*> &dashboards ): repo(repo) ,dashboards(dashboards){}

    void execute(int id,std::string sourceUI ) {
        auto* device = repo->getDeviceById(id);
        if (!device) return;

        if (device->lightEntity.getState()) {
            device->lightEntity.turnOff();
        } else {
            device->lightEntity.turnOn();
        }

        device->executeConnection->execute(device->lightEntity, device->lightConnectionData);


        for(auto *dashboard : dashboards)
        {
            if(sourceUI==dashboard->getName())
                        continue;
            dashboard->updateLightState(device->lightEntity);
        }
    }

};