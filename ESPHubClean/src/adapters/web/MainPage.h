#pragma once
#include <WebServer.h>
#include <ports/IRepoPort.h>
#include <ports/IRepoSwitch.h>
#include <ArduinoJson.h>
#include "ControlSourcesPage.h"

class MainPage
{
private:
  String html;
  String table;
  WebServer server;
  uint32_t nextId = 1;

  bool parseMac(const String &macStr, uint8_t out[6])
  {
    int values[6];
    if (sscanf(macStr.c_str(), "%x:%x:%x:%x:%x:%x",
               &values[0], &values[1], &values[2],
               &values[3], &values[4], &values[5]) != 6)
    {
      return false;
    }

    for (int i = 0; i < 6; i++)
      out[i] = (uint8_t)values[i];

    return true;
  }

  void getTable()
  {

    server.sendContent(R"rawliteral(
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

    server.sendContent("</table>");
  }

public:
  MainPage() : server(80)
  {
  }

  void showMainPage()
  {
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");

    server.sendContent(R"rawliteral(<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <title>Smart Home Dashboard</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <style>
     



    </style>
  </head>

  <body>
    <!-- SIDEBAR -->
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

    <!-- MAIN -->
    <div class="main">
      <!-- TOPBAR -->
      <div class="topbar">
          <span class="status">● Connected</span>
      </div>
      <div id="content">)rawliteral");

    getTable();

    server.sendContent(R"rawliteral(
      <!-- GRID -->
    </div>
    </div>

    <!-- MODAL -->
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

    <!-- TOAST -->
    <div class="toast" id="toast"></div>

    <script>

      /* ================== MODAL ================== */

        let selectedItem = "Devices";
    function showItem(url)
    {
    switch (url) {
        case "/showControlSources":
            selectedItem = "Control Sources";
            break;

        case "/showDevices":
            selectedItem = "Devices";
            break;

        case "/showRooms":
            selectedItem = "Rooms";
            break;

        case "/showControllers":
            selectedItem = "Controllers";
            break;

        case "/showProtocols":
            selectedItem = "Protocols";
            break;

        default:
            selectedItem = "";
    }
    fetch(url)
    .then(response=>response.text())
    .then(html=>showContent(html))
        }

      function showContent(html) {
   
        document.getElementById("content").innerHTML = html;
    }
      function openModal() {
        document.getElementById("modal").style.display = "flex";
        onProtocolChange();
      }

      function closeModal() {
        document.getElementById("modal").style.display = "none";
        clearForm();
      }

      function clearForm() {
        document.getElementById("name").value = "";
        document.getElementById("room").value = "";
        document.getElementById("controllerId").value = "";
        document.getElementById("protocol").value = "GPIO";
        document.getElementById("mac").value = "";
        document.getElementById("pin").value = "";
        onProtocolChange();
      }

      function onProtocolChange() {
        const protocol = document.getElementById("protocol").value;
        const macGroup = document.getElementById("macGroup");
        const macInput = document.getElementById("mac");

        if (protocol === "ESP-NOW") {
          macGroup.classList.remove("hidden");
          macInput.required = true;
        } else {
          macGroup.classList.add("hidden");
          macInput.required = false;
          macInput.value = "";
        }
      }

      function isValidMac(mac) {
        return /^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$/.test(mac.trim());
      }

      function showToast(msg) {
        let t = document.getElementById("toast");
        t.innerText = msg;
        t.style.display = "block";
        setTimeout(() => (t.style.display = "none"), 2000);
      }



      function addDeviceEntity() {

    const name = document.getElementById("name").value.trim();
    const room = document.getElementById("room").value.trim();
    const controllerId = document.getElementById("controllerId").value.trim();
    const protocol = document.getElementById("protocol").value;
    const mac = document.getElementById("mac").value.trim();
    const pin = document.getElementById("pin").value.trim();
    const switchPin = document.getElementById("switchPin").value.trim();

    if (!name || !room || !controllerId || !pin || !switchPin) {
        showToast("Please fill all required fields");
        return;
    }

    if (protocol === "ESP-NOW" && !isValidMac(mac)) {
        showToast("Enter a valid MAC like AA:BB:CC:DD:EE:FF");
        return;
    }

    const payload = {
        name,
        room,
        controllerId: parseInt(controllerId),
        protocol,
        mac: protocol === "ESP-NOW" ? mac : "",
        pin: parseInt(pin),
        switchPin: parseInt(switchPin),
        state: false
    };

    fetch("/api/device/add", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(payload)
    })
    .then(async (res) => {

        if (!res.ok) {
            const msg = await res.text();
            throw new Error(msg || "Failed to add device");
        }

        return res.text();
    })
    .then(() => {

        closeModal();
        showToast("Device added");

        return fetch("/api/switch/add", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                switchPin: parseInt(switchPin)
            })
        });
    })
    .then(async (res) => {

        if (!res.ok) {
            const msg = await res.text();
            throw new Error(msg || "Failed to add switch");
        }

        return res.text();
    })
    .then(() => {
        showToast("Device and Switch added successfully");
        location.reload();
    })
    .catch((err) => {
        showToast(err.message);
    });
}


     function addDevice() {

    switch (selectedItem) {

        case "Devices":
            addDeviceEntity();
            break;

        case "Control Sources":
            addControlSourceEntity();
            break;

        case "Controllers":
            addControllerEntiy();
            break;

        case "Rooms":
            addRoomEntity();
            break;

        case "Protocols":
            addProtocolEntity();
            break;

        default:
            showToast("Unknown item selected");
            break;
    }
}


     function showControlSources()
     {
     fetch("/showControlSources",HTTP_GET);
     }

      function deleteDevice(id) {
  if (!confirm("Delete this device?")) return;

  fetch("/api/device/delete", {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({ id: id })
  })
  .then(async (res) => {
    if (!res.ok) {
      const txt = await res.text();
      throw new Error(txt || "Delete failed");
    }
    showToast("Device deleted");
    location.reload();
  })
  .catch((err) => {
    showToast(err.message);
  });
}
  
      /* INIT */
    </script>
  </body>
</html>
)rawliteral");
  }

  void addDevice()
  {
    String body = server.arg("plain");

    Serial.println("API HIT");
    Serial.println(body);
    DynamicJsonDocument doc(512);

    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    String name = doc["name"].as<String>();

    String room = doc["room"].as<String>();

    int controllerId = doc["controllerId"] | 0;

    String protocol = doc["protocol"].as<String>();
    String mac = doc["mac"].as<String>();

    int pin = doc["pin"] | 0;

    Serial.println("Heap befor ");
    Serial.println(ESP.getFreeHeap());

    if (protocol == "GPIO")
    {
    }
    else if (protocol == "ESP-NOW")
    {
      uint8_t macBytes[6];
      if (!parseMac(mac, macBytes))
      {
        server.send(400, "text/plain", "Invalid MAC");
        return;
      }
    }
    else
    {
      server.send(400, "text/plain", "Unknown protocol");
      return;
    }

    server.send(200, "text/plain", "OK");
  }

  void deleteDevice()
  {
    String body = server.arg("plain");
    DynamicJsonDocument doc(256);

    DeserializationError error = deserializeJson(doc, body);
    if (error)
    {
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }
    uint32_t id = doc["id"] | 0;

    bool removed = false;

    if (!removed)
    {
      server.send(404, "text/plain", "Device not found");
      return;
    }

    server.send(200, "text/plain", "OK");
  }

  void addSwitch()
  {
    String body = server.arg("plain");

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    uint32_t deviceId = nextId - 1;
    int switchPin = doc["switchPin"] | -1;

    if (deviceId == 0 || switchPin < 0)
    {
      server.send(400, "text/plain", "Missing or invalid fields");
      return;
    }

    server.send(200, "text/plain", "OK");
  }

  void begin()
  {

    WiFi.begin("Ali", "111111111");
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Connecting");
      delay(500);
    }
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [this]
              { showMainPage(); });

    server.on("/showControlSources", HTTP_GET, [this]()
              { ControlSourcesPage::show(&server); });
    server.on("/showDevices", HTTP_GET, []() {});
    server.on("/showRooms", HTTP_GET, []() {});
    server.on("/api/device/add", HTTP_POST, [this]()
              { addDevice(); });

    server.on("/api/device/delete", HTTP_POST, [this]()
              { deleteDevice(); });

    server.on("/api/switch/add", HTTP_POST, [this]() {});

    server.begin();
  }

  void handle()
  {
    server.handleClient();
  }
};
