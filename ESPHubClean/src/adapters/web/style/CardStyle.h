#pragma once
#include <WebServer.h>

class CardStyle
{
public:
    static void sendStyle()
    {
        EspServer::server.sendContent(R"rawliteral(
            .card {
                background: rgba(255, 255, 255, 0.05);
                padding: 20px;
                border-radius: 20px;
                backdrop-filter: blur(20px);
                transition: 0.3s;
                position: relative;
            }

            .card:hover {
                transform: translateY(-5px);
            }

            .card.active {
                box-shadow: 0 0 20px #22c55e;
            }
        )rawliteral");
    }
};