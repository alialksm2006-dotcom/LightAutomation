#pragma once
class FormStyle
{
public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(
            .form-grid {
                display: grid;
                grid-template-columns: 1fr 1fr;
                gap: 14px;
            }

            .form-group {
                display: flex;
                flex-direction: column;
                gap: 8px;
            }

            .form-group label {
                color: #cbd5e1;
                font-size: 0.9rem;
            }

            .form-group input,
            .form-group select {
                width: 100%;
                padding: 12px 14px;
                border-radius: 12px;
                border: 1px solid rgba(255, 255, 255, 0.1);
                outline: none;
                background: rgba(255, 255, 255, 0.05);
                color: white;
                transition: 0.25s;
            }

            .form-group input:focus,
            .form-group select:focus {
                border-color: #38bdf8;
                box-shadow: 0 0 0 3px rgba(56, 189, 248, 0.15);
            }

            .form-group select option {
                background: #111827;
                color: white;
            }

            .hidden {
                display: none;
            }

            @media (max-width: 640px) {
                .form-grid {
                    grid-template-columns: 1fr;
                }
            }
        )rawliteral");
    }
};