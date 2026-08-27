#pragma once
#include "ports/IRepoPort.h"
#include "ports/ILightExecuteConnection.h"
#include <TelnetStream.h>
#include "ports/IDashboardPort.h"

#include <vector>
class TurnOnLightUseCase {
IRepoPort *repo;
    std::vector<IDashboardPort*> &dashboards;

public:
    TurnOnLightUseCase( IRepoPort *repo,std::vector<IDashboardPort*> &dashboards) : repo(repo),dashboards(dashboards)
         {}

    void execute( int id ,std::string sourceUI) {
        IRepoPort::SmartDevice* light = repo->getDeviceById(id);
        if(light==nullptr||light->lightEntity.getState())
            return;
            if(light->lightEntity.getState()) return ;
            light->lightEntity.turnOn();
            light->executeConnection->execute(light->lightEntity, light->lightConnectionData);

            

        for(auto *dashboard : dashboards)
        {
            if(sourceUI==dashboard->getName())
                        break;
            dashboard->updateLightState(light->lightEntity);
        }
    }
};