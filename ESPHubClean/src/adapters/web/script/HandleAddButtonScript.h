#pragma once
#include "adapters/api/EspServer.h"
class HandleAddButtonScript
{public:
static void send()
{
EspServer::server.sendContent(R"rawliteral(
function handleAdd() {
    if (selectedItem === "Wireless") {
        startWirelessDiscovery();
        return;
    }
    if (!entityRoutes[selectedItem]) {
        showToast("Select an item first", true);
        return;
    }
    openEntityModal();
}
    )rawliteral");

    
}
};