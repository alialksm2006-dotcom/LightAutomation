#pragma once
#include <WebServer.h>

class BadgeStyle
{
public:
    static void sendStyle()
    {
        EspServer::server.sendContent(R"rawliteral(
            .state-badge {
                padding: 4px 10px;
                border-radius: 12px;
                color: white;
                font-weight: bold;
                font-size: 0.8rem;
                display: inline-block;
                text-align: center;
            }

            .state-badge.on {
                background: linear-gradient(135deg, #22c55e, #16a34a);
            }

            .state-badge.off {
                background: linear-gradient(135deg, #ef4444, #b91c1c);
            }

            .switch-badge {
                background: rgba(56, 189, 248, 0.15);
                color: #38bdf8;
                padding: 4px 10px;
                border-radius: 8px;
                font-size: 0.8rem;
                border: 1px solid rgba(56, 189, 248, 0.3);
                display: inline-block;
                white-space: nowrap;
            }

            .info {
                white-space: pre-line;
            }
        )rawliteral");
    }
};