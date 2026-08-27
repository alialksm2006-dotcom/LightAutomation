#pragma once
#include "ports/ILightExecuteConnection.h"
#include "ports/IDashboardPort.h"
#include <vector>
class TurnOffLightUseCase
{
private:
    IRepoPort *repo;
        std::vector<IDashboardPort*> &dashboards;


public:
    TurnOffLightUseCase(IRepoPort *repo,std::vector<IDashboardPort*> &dashboards ) : repo(repo),dashboards(dashboards) {}

    void execute(int id,std::string sourceUI)
    {

        IRepoPort::SmartDevice *light = repo->getDeviceById(id);
        if (light == nullptr||!light->lightEntity.getState())
            return;
        light->lightEntity.turnOff();
        light->executeConnection->execute(light->lightEntity, light->lightConnectionData);

        
        for(auto *dashboard : dashboards)
        {
            if(sourceUI==dashboard->getName())
                        break;

            dashboard->updateLightState(light->lightEntity);
             
        }
    }
};