#pragma once
#include "EspServer.h"
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
        EspServer::server.on("/showControlSources",HTTP_GET,show);
    }
    
    
};