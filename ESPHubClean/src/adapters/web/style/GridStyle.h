#pragma once
#include <WebServer.h>

class GridStyle
{
public:
    static void sendStyle(WebServer* server)
    {
        server->sendContent(R"rawliteral(
            .grid {
                display: grid;
                grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
                gap: 20px;
            }
        )rawliteral");
    }
};