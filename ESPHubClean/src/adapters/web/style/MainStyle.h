#pragma once
#include <WebServer.h>
class MainStyle
{
    static void sendStyle(WebServer* server)
    {
        server->sendContent(R"rawliteral(
  .main {
    flex: 1;
    padding: 20px;
    margin-left: 250px;
  }
)rawliteral");
    }
};