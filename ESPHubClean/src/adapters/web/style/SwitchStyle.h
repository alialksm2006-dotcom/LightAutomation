#pragma once
#include <WebServer.h>

class SwitchStyle
{
public:
    static void sendStyle(WebServer* server)
    {
        server->sendContent(R"rawliteral(
            .switch {
                width: 50px;
                height: 25px;
                background: #64748b;
                border-radius: 50px;
                position: relative;
                cursor: pointer;
            }

            .switch::after {
                content: "";
                width: 20px;
                height: 20px;
                background: white;
                position: absolute;
                top: 2.5px;
                left: 3px;
                border-radius: 50%;
                transition: 0.3s;
            }

            .switch.active {
                background: #22c55e;
            }

            .switch.active::after {
                left: 26px;
            }
        )rawliteral");
    }
};