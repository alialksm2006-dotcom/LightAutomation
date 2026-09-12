#pragma once
#include <WebServer.h>
class SideBarhtml{
static void sendHtml(WebServer* server)
{
    server->sendContent(R"rawliteral(
        
    <div class="sidebar">
      <h2>Smart Home</h2>
      <ul>
        <li onclick="showDevices()">Devices</li>
        <li onclick="showRooms()">Rooms</li>
        <li onclick="showItem('/showControlSources')">Control Sources</li>
        <li onclick="showControllers()">Controllers</li>
        <li onclick="showProtocols()">Protocols</li>
      </ul>
    </div>

        )rawliteral");
}

};