#pragma once
#include "adapters/api/EspServer.h"
class SideBarhtml{
  public:
static void sendHtml()
{
    EspServer::server.sendContent(R"rawliteral(
        
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