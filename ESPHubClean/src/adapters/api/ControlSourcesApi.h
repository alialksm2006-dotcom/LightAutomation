#pragma once
#include "EspServer.h"
#include "adapters/web/html/ControlSourcesPage.h"
class ControlSourceApi
{
    private:
     static void add()
    {
     
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
        EspServer::server.on("/ControlSources/add",HTTP_POST,add);
        EspServer::server.on("/ControlSources/showAdd",HTTP_GET,ControlSourcesPage::showAdd);
        EspServer::server.on("/showControlSources",HTTP_GET,ControlSourcesPage::show);
    }
    
    
};