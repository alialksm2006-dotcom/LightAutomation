#pragma once
#include <WebServer.h>

class ToastStyle
{
public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(
            .toast {
                position: fixed;
                bottom: 20px;
                right: 20px;
                background: #22c55e;
                padding: 10px 20px;
                border-radius: 10px;
                display: none;
            }
        )rawliteral");
    }
};