#pragma once
#include <WebServer.h>

class TopBarStyle
{
public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(
            .topbar {
                display: flex;
                justify-content: space-between;
                margin-bottom: 20px;
            }

            .status {
                color: #22c55e;
            }
        )rawliteral");
    }
};