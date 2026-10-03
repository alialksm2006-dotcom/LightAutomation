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
                align-items: center;
                gap: 16px;
                margin-bottom: 20px;
                padding: 12px 16px;
                border: 1px solid rgba(148, 163, 184, .14);
                border-radius: 16px;
                background: rgba(15, 23, 42, .72);
                box-shadow: 0 8px 24px rgba(0, 0, 0, .14);
            }

            .status {
                color: #22c55e;
                display: inline-flex;
                align-items: center;
                gap: 8px;
                white-space: nowrap;
                font-size: .9rem;
            }

            .status::first-letter {
                color: #22c55e;
            }

            .topbar-actions {
                display: flex;
                align-items: center;
                gap: 10px;
                margin-left: auto;
            }

            .topbar .add-action {
                min-height: 42px;
                padding: 10px 18px;
                border-radius: 12px;
                font-weight: 650;
                box-shadow: 0 6px 18px rgba(56, 189, 248, .18);
            }

            .toolbar-menu {
                position: relative;
            }

            .toolbar-menu-toggle {
                width: 42px;
                height: 42px;
                padding: 0;
                border: 1px solid rgba(148, 163, 184, .22);
                border-radius: 12px;
                color: #dbeafe;
                background: rgba(30, 41, 59, .8);
                font-size: 1.35rem;
                line-height: 1;
                cursor: pointer;
                transition: background .18s ease, border-color .18s ease, transform .18s ease;
            }

            .toolbar-menu-toggle:hover,
            .toolbar-menu.open .toolbar-menu-toggle {
                background: rgba(51, 65, 85, .95);
                border-color: rgba(56, 189, 248, .4);
            }

            .toolbar-menu-toggle:active {
                transform: scale(.95);
            }

            .toolbar-menu-panel {
                position: absolute;
                z-index: 1100;
                top: calc(100% + 10px);
                right: 0;
                width: 220px;
                padding: 7px;
                border: 1px solid rgba(148, 163, 184, .2);
                border-radius: 14px;
                background: #111c2e;
                box-shadow: 0 18px 42px rgba(0, 0, 0, .4);
            }

            .toolbar-menu-panel[hidden] {
                display: none;
            }

            .toolbar-menu-item {
                display: flex;
                align-items: center;
                gap: 11px;
                width: 100%;
                padding: 11px 12px;
                border: 0;
                border-radius: 9px;
                color: #dbe5f2;
                background: transparent;
                text-align: left;
                font: inherit;
                cursor: pointer;
                transition: background .16s ease, color .16s ease;
            }

            .toolbar-menu-item:hover {
                color: white;
                background: rgba(148, 163, 184, .13);
            }

            .toolbar-menu-item .menu-icon {
                width: 20px;
                color: #7dd3fc;
                text-align: center;
            }

            .toolbar-menu-divider {
                height: 1px;
                margin: 6px 4px;
                background: rgba(148, 163, 184, .16);
            }

            .toolbar-menu-item.logout-action,
            .toolbar-menu-item.logout-action .menu-icon {
                color: #fca5a5;
            }

            .toolbar-menu-item.logout-action:hover {
                background: rgba(239, 68, 68, .12);
            }

            @media (max-width: 600px) {
                .topbar {
                    padding: 10px 12px;
                    gap: 10px;
                }

                .topbar .status {
                    font-size: .8rem;
                }

                .topbar .add-action {
                    min-height: 40px;
                    padding: 9px 13px;
                }

                .toolbar-menu-toggle {
                    width: 40px;
                    height: 40px;
                }
            }


            
        )rawliteral");
    }
};