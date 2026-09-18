#pragma once
#include "EspServer.h"
#include "adapters/web/html/ControlSourcesPage.h"
#include "adapters/data_manager/ControllerSourceStorage.h"
class ControlSourceApi
{
private:
    static void add()
    {
        if (EspServer::server.hasArg("controllerId") && EspServer::server.hasArg("pinNumber"))
        {
            String pinStr = EspServer::server.arg("pinNumber");
            String cntrlerIdStr = EspServer::server.arg("controllerId");

            int pinNumber = pinStr.toInt();
            int controllerId = cntrlerIdStr.toInt();
            if (pinNumber != -1)
            {
                if (ControllerSourceStorage::isEsixt(controllerId))
                {
                    EspServer::server.send(400, "text/plain", "please select found controller id,or enter -1");
                    return;

                }
            }
            ControllerSource source;
            source.setPinNumber(pinNumber);
            source.setControllerId(controllerId);

            if(ControllerSourceStorage::add(source))
            {
             EspServer::server.send(200, "text/plain", "Control Source Added Successfully");

            }
            else EspServer::server.send(200, "text/plain", "Control Source Added Failed!");


        }
        else
            EspServer::server.send(400, "text/plain", "Bad Request: Missing Parameters");
    }
    static void show()
    {
        EspServer::server.sendContent("<table class=\"table\">");
        EspServer::server.sendContent("<th>ID</th><th>RelayNumber</th><th>Controller ID</th>");
        EspServer::server.sendContent("</table>");
    }

public:
    static void begin()
    {
        EspServer::server.on("/ControlSources/add", HTTP_POST, add);
        EspServer::server.on("/ControlSources/showAdd", HTTP_GET, ControlSourcesPage::showAdd);
        EspServer::server.on("/showControlSources", HTTP_GET, ControlSourcesPage::show);
    }
};