#pragma once
#include "Server.h"
class ControlSourceApi
{
    private:
     static void add()
    {
     
    }
    public:
   static void addControlSourceApi()
    {
        Server::server.on("/ControlSources/add",HTTP_POST,add);
    }
};