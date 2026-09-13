#pragma once
#include "adapters/api/EspServer.h"
class LightsPage
{
public:
    static void sendTable()
  {
   EspServer::server.sendContent(R"rawliteral(
<table>
  <tr>
    <th>ID</th>
    <th>Name</th>
    <th>Room</th>
    <th>Controller ID</th>
    <th>State</th>
    <th>Protocol</th>
    <th>RelayPin</th>
    <th>Number On Device</th>   
    <th>Controller</th>
    <th>Control Sources</th>
    <th>Action</th>
  </tr>
)rawliteral");

    // for (; ; )
    // {
    //   server.sendContent("<tr>");
    //   server.sendContent("<td>" + String(device.lightEntity.id) + "</td>");
    //   server.sendContent("<td>" + String(device.lightEntity.name.c_str()) + "</td>");
    //   server.sendContent("<td>" + String(device.lightEntity.room.c_str()) + "</td>");
    //   server.sendContent("<td>" + String(device.lightEntity.controllerId) + "</td>");
    //   server.sendContent("<td><span class='state-badge " + String(device.lightEntity.state ? "on" : "off") + "'>" + String(device.lightEntity.state ? "ON" : "OFF") + "</span></td>");
    //   server.sendContent("<td>" + String(device.lightConnectionData->getProtocol().c_str()) + "</td>");
    //   server.sendContent("<td class='info'>" + String(device.lightConnectionData->connectionString().c_str()) + "</td>");
    //   ISwitch *sw = switches->getSwitchByDeviceId(device.lightEntity.id);
    //   server.sendContent("<td><span class='switch-badge'>" + String(sw ? sw->getType().c_str() : "NULL") + "</span></td>");
    //   server.sendContent("<td><button class='btn delete-btn' onclick='deleteDevice(" + String(device.lightEntity.id) + ")'>Delete</button></td>");
    //   server.sendContent("</tr>");
    // }

    EspServer::server.sendContent("</table>");
  }


  static void sendAddLightModal( )
  {
EspServer::server.sendContent(R"rawliteral(
    
    <div class="modal" id="modal">
      <div class="modal-content">
        <div class="modal-header">
          <h3>Add Device</h3>
          <button class="close-btn" onclick="closeModal()">×</button>
        </div>

        <div class="form-grid">
          <div class="form-group">
            <label for="name">Name</label>
            <input id="name" placeholder="Device Name" />
          </div>

          <div class="form-group">
            <label for="room">Room</label>
            <input id="room" placeholder="Room" />
          </div>

          <div class="form-group">
            <label for="controllerId">Sub Controller</label>
            <input id="controllerId" type="number" min="0" placeholder="Controller ID" />
          </div>

          <div class="form-group">
            <label for="protocol">Protocol</label>
            <select id="protocol" onchange="onProtocolChange()">
              <option value="GPIO">GPIO</option>
              <option value="ESP-NOW">ESP-NOW</option>
            </select>
          </div>

          <div class="form-group hidden" id="macGroup">
            <label for="mac">MAC Address</label>
            <input id="mac" placeholder="AA:BB:CC:DD:EE:FF" maxlength="17" />
          </div>

          <div class="form-group">
            <label for="pin">Pin</label>
            <input id="pin" type="number" min="0" max="255" placeholder="Pin number" />
          </div>
        </div>
         <div class="form-group">
  <label for="switchPin">Switch Number</label>
  <input id="switchPin" type="number" min="0" max="255" placeholder="Switch number" />
</div>

        <div class="modal-actions">
          <button class="btn secondary" onclick="closeModal()">Cancel</button>
          <button class="btn" onclick="addDevice()">Save</button>
        </div>
      </div>
    </div>
    )rawliteral");
  }

};