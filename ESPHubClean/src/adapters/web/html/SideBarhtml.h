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
        <li onclick="showItem('/showDevices')">Devices</li>
        <li onclick="showItem('/showRooms')">Rooms</li>
        <li>
        <span onclick="showItem('/showControlSources')">Control Sources</span>
          <ul>
           <li onclick="showItem('/showButtons')">Buttons</li>
          </ul>
          </li>
        <li onclick="showItem('/showControllers')">Controllers</li>
        <li>
          <span onclick="showItem('/showProtocols')">Protocols</span>
          <ul>
            <li onclick="showItem('/showWireless')">Wireless</li>
          </ul>
        </li>
      </ul>
    </div>

        )rawliteral");
}

};