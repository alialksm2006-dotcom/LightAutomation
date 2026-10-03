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
        <li class="nav-item" data-nav-route="/showDevices" onclick="showItem('/showDevices')">Devices</li>
        <li class="nav-item" data-nav-route="/showRooms" onclick="showItem('/showRooms')">Rooms</li>
        <li>
        <span class="nav-item" data-nav-route="/showControlSources" onclick="showItem('/showControlSources')">Control Sources</span>
          <ul>
           <li class="nav-item" data-nav-route="/showButtons" onclick="showItem('/showButtons')">Buttons</li>
          </ul>
          </li>
        <li class="nav-item" data-nav-route="/showControllers" onclick="showItem('/showControllers')">Controllers</li>
        <li>
          <span class="nav-item" data-nav-route="/showProtocols" onclick="showItem('/showProtocols')">Protocols</span>
          <ul>
            <li class="nav-item" data-nav-route="/showWireless" onclick="showItem('/showWireless')">Wireless</li>
          </ul>
        </li>
      </ul>
    </div>

        )rawliteral");
}

};