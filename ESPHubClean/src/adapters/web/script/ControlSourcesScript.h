#pragma once
#include <WebServer.h>
class ControlSourcesScript
{
public: 
static void send(WebServer* server)
{
    server->sendContent(R"rawliteral(
        function 
        )rawliteral");
}
};