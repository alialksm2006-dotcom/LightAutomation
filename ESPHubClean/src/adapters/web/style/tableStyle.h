#pragma once
#include <WebServer.h>

class TableStyle
{
public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(
            table {
                width: 100%;
                border-collapse: collapse;
                margin-top: 12px;
                background: #111827;
                border-radius: 12px;
                overflow: hidden;
                border-collapse: separate;
                border-spacing: 0;
                box-shadow: 0 10px 30px rgba(0, 0, 0, 0.25);
            }

            table,
            th,
            td {
                border: none;
            }

            th,
            td {
                padding: 10px 12px;
                text-align: left;
                border-bottom: 0.2px solid rgba(255, 255, 255, 0.1);
                font-family: "Segoe UI", Arial, sans-serif;
                font-size: 0.9rem;
            }

            td {
                color: #e5e7eb;
            }

            th {
                background: #22c55e;
                color: #000000;
                font-weight: 600;
                text-transform: uppercase;
                letter-spacing: 0.5px;
            }

            tr:nth-child(even) {
                background: rgba(255, 255, 255, 0.03);
            }

            tr:hover {
                background: rgba(56, 189, 248, 0.15);
                cursor: default;
            }
        )rawliteral");
    }
};