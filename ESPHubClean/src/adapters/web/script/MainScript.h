#pragma once
#include <WebServer.h>
class MainScript
{
public :
static void sendMainScript()
{
     EspServer::server.sendContent(R"rawliteral(
         

let selectedItem = "Devices";
const entityRoutes = {
    "Devices": "/showDevices",
    "Rooms": "/showRooms",
    "Controllers": "/showControllers",
    "Protocols": "/showProtocols",
    "Wireless": "/showWireless",
    "Control Sources": "/showControlSources",
    "Buttons": "/showButtons"
};

async function showItem(url) {
    selectedItem = Object.keys(entityRoutes).find(name => entityRoutes[name] === url) || "";
    try {
        const response = await fetch(url);
        if (!response.ok) throw new Error("Could not load " + selectedItem);
        document.getElementById("content").innerHTML = await response.text();
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

document.addEventListener("DOMContentLoaded", () => showItem(entityRoutes.Devices));
  
        )rawliteral");
}
};