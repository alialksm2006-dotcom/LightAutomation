#pragma once
#include <WebServer.h>

class ButtonStyle
{
public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(

.btn {
    padding: 10px 16px;
    border-radius: 12px;
    border: none;
    cursor: pointer;
    background: linear-gradient(135deg, #38bdf8, #6366f1);
    color: white;
    transition: 0.3s;
}

.btn:hover {
    transform: scale(1.05);
}

.btn:active,
.btn.is-pressed {
    transform: scale(.95);
    filter: brightness(1.2);
    box-shadow: 0 0 0 3px rgba(56, 189, 248, .2);
}

.btn.secondary {
    background: rgba(255, 255, 255, 0.08);
}

.btn.secondary:hover {
    transform: scale(1.05);
}

.delete-btn {
    padding: 8px 14px;
    border-radius: 10px;
    border: none;
    cursor: pointer;
    background: linear-gradient(135deg, #ef4444, #b91c1c);
    color: white;
    font-size: 0.85rem;
    transition: 0.25s;
    box-shadow: 0 4px 12px rgba(239, 68, 68, 0.25);
}

.delete-btn:hover {
    transform: scale(1.05);
    box-shadow: 0 6px 16px rgba(239, 68, 68, 0.35);
}

.delete-btn:active,
.delete-btn.is-pressed {
    transform: scale(.95);
    filter: brightness(1.18);
    box-shadow: 0 0 0 3px rgba(239, 68, 68, .2);
}

button,
summary {
    -webkit-tap-highlight-color: transparent;
}

button:focus-visible,
summary:focus-visible,
.nav-item:focus-visible {
    outline: 2px solid #38bdf8;
    outline-offset: 3px;
}

)rawliteral");
    }
};