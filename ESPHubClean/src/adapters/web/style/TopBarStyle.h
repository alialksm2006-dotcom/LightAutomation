#pragma once
#include <WebServer.h>

class TopbarStyle
{
public:
    static void sendStyle(WebServer* server)
    {
        server->sendContent(R"rawliteral(
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