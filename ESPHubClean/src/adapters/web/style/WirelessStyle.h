#pragma once

#include "adapters/api/EspServer.h"

class WirelessStyle
{
public:
    static void sendStyle()
    {
        EspServer::server.sendContent(R"rawliteral(
.discovery-status {
    display: flex;
    align-items: center;
    gap: 14px;
    padding: 12px 0;
}

.discovery-spinner {
    width: 28px;
    height: 28px;
    border: 3px solid rgba(255, 255, 255, 0.18);
    border-top-color: #38bdf8;
    border-radius: 50%;
    animation: wireless-spin 0.8s linear infinite;
    flex: 0 0 auto;
}

@keyframes wireless-spin {
    to { transform: rotate(360deg); }
}

#wirelessDiscoveryResults {
    max-height: 45vh;
    overflow: auto;
}

#wirelessDiscoveryResults tr:has(input:checked) {
    outline: 2px solid #38bdf8;
}
)rawliteral");
    }
};
