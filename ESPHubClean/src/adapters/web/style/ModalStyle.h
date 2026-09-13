#pragma once
#include <WebServer.h>

class ModalStyle
{
public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(
            .modal {
                position: fixed;
                inset: 0;
                background: rgba(0, 0, 0, 0.72);
                display: none;
                align-items: center;
                justify-content: center;
                z-index: 1000;
                padding: 16px;
            }

            .modal-content {
                width: min(95vw, 520px);
                background: rgba(17, 24, 39, 0.98);
                border: 1px solid rgba(255, 255, 255, 0.08);
                border-radius: 20px;
                padding: 20px;
                box-shadow: 0 20px 50px rgba(0, 0, 0, 0.4);
            }

            .modal-header {
                display: flex;
                align-items: center;
                justify-content: space-between;
                margin-bottom: 18px;
            }

            .modal-header h3 {
                color: #38bdf8;
                font-size: 1.2rem;
            }

            .close-btn {
                width: 38px;
                height: 38px;
                border: none;
                border-radius: 12px;
                background: rgba(255, 255, 255, 0.08);
                color: white;
                font-size: 1.5rem;
                cursor: pointer;
                transition: 0.25s;
            }

            .close-btn:hover {
                background: rgba(255, 255, 255, 0.16);
            }

            .modal-actions {
                display: flex;
                justify-content: flex-end;
                gap: 10px;
                margin-top: 18px;
            }
        )rawliteral");
    }
};