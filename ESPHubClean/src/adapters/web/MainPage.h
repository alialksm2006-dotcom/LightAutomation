#pragma once
#include <WebServer.h>
#include <ports/IRepoPort.h>
#include <ports/IRepoSwitch.h>
#include <ArduinoJson.h>

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
  MainPage( ) : server(80)
  {
  }

  void begin()
  {
    
    server.on("/", HTTP_GET, [this]
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
      /* ================== GLOBAL ================== */
      /* النجمة تعني تحديد جميع العناصر في الصفحة 
      أي قاعدة توضع هنا ستطبق على كل شيء: <div>, <p>, <h1>, <button>… */
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
        font-family: "Segoe UI", sans-serif;
      }

    body {
  background: linear-gradient(135deg, #0f172a, #020617);
  color: white;
  display: flex;
  margin: 0;
  padding: 0;   
}

      /* ================== SIDEBAR ================== */
    .sidebar {
    width: 250px;
    position: fixed;
    top: 0;
    left: 0;
    bottom: 0;
    padding: 20px;
    border-right: 1px solid rgba(255, 255, 255, 0.1);

    /* اللون ثابت شبه شفاف بدون blur */
    background-color: rgba(17, 24, 39, 0.95); 

    overflow-y: auto;

    /* جبر إنشاء طبقة compositing لتقليل وميض */
    transform: translate3d(0,0,0);
    will-change: transform, background-color;
}

html, body {
    overscroll-behavior: none; /* يمنع ال-bounce عند التمرير */
    -webkit-overflow-scrolling: auto; /* لأجهزة iOS */
}
      /* ================== STATE BADGE ================== */
.state-badge {
    padding: 4px 10px;
    border-radius: 12px;
    color: white;
    font-weight: bold;
    font-size: 0.8rem;
    display: inline-block;
    text-align: center;
}

.state-badge.on {
    background: linear-gradient(135deg, #22c55e, #16a34a); /* أخضر */
}

.state-badge.off {
    background: linear-gradient(135deg, #ef4444, #b91c1c); /* أحمر */
}
      .info{
      white-space:pre-line;
      }
      .sidebar h2 {
        margin-bottom: 30px;
        color: #38bdf8;
      }

      .sidebar ul {
        list-style: none;
      }

      .sidebar li {
        padding: 12px;
        margin-bottom: 10px;
        border-radius: 10px;
        cursor: pointer;
        transition: 0.5s;
      }

      .sidebar li:hover {
        background: rgba(255, 255, 255, 0.1);
        transform: scale(1.05);
      }

      /* ================== MAIN ================== */
      .main {
    flex: 1;
    padding: 20px;
    margin-left: 250px;   /* نفس عرض الـ sidebar */
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

      /* ================== TOPBAR ================== */
      .topbar {
        display: flex;
        justify-content: space-between;
        margin-bottom: 20px;
      }

      .status {
        color: #22c55e;
      }

      /* ================== BUTTON ================== */
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

      /* ================== GRID ================== */
      .grid {
        display: grid;
        grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
        gap: 20px;
      }

      /* ================== CARD ================== */
      .card {
        background: rgba(255, 255, 255, 0.05);
        padding: 20px;
        border-radius: 20px;
        backdrop-filter: blur(20px);
        transition: 0.3s;
        position: relative;
      }

      .card:hover {
        transform: translateY(-5px);
      }

      .card.active {
        box-shadow: 0 0 20px #22c55e;
      }

      /* ================== TOGGLE ================== */
      .switch {
        width: 50px;
        height: 25px;
        background: gray;
        border-radius: 50px;
        position: relative;
        cursor: pointer;
      }

      .switch::after {
        content: "";
        width: 20px;
        height: 20px;
        background: white;
        position: absolute;
        top: 2.5px;
        left: 3px;
        border-radius: 50%;
        transition: 0.3s;
      }

      .switch.active {
        background: #22c55e;
      }

      .switch.active::after {
        left: 26px;
      }

      /* ================== MODAL ================== */
       .modal {
    position: fixed;
    inset: 0;
    background: rgba(0, 0, 0, 0.72);
    display: none;
    align-items: center;
    justify-content: center;
    z-index: 1000;
    padding: 16px;
  }

      .modal-content {
    width: min(95vw, 520px);
    background: rgba(17, 24, 39, 0.98);
    border: 1px solid rgba(255, 255, 255, 0.08);
    border-radius: 20px;
    padding: 20px;
    box-shadow: 0 20px 60px rgba(0, 0, 0, 0.45);
  }

  .modal-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-bottom: 18px;
  }

  .modal-header h3 {
    color: #38bdf8;
    font-size: 1.2rem;
  }

  .close-btn {
    width: 38px;
    height: 38px;
    border: none;
    border-radius: 12px;
    background: rgba(255, 255, 255, 0.08);
    color: white;
    font-size: 1.5rem;
    cursor: pointer;
    transition: 0.25s;
  }

  .close-btn:hover {
    background: rgba(255, 255, 255, 0.16);
  }

  .form-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 14px;
  }

  .form-group {
    display: flex;
    flex-direction: column;
    gap: 8px;
  }

  .form-group label {
    color: #cbd5e1;
    font-size: 0.9rem;
  }

  .form-group input,
  .form-group select {
    width: 100%;
    padding: 12px 14px;
    border-radius: 12px;
    border: 1px solid rgba(255, 255, 255, 0.1);
    outline: none;
    background: rgba(255, 255, 255, 0.05);
    color: white;
    transition: 0.25s;
  }

  .form-group input:focus,
  .form-group select:focus {
    border-color: #38bdf8;
    box-shadow: 0 0 0 3px rgba(56, 189, 248, 0.15);
  }

  .form-group select option {
    background: #111827;
    color: white;
  }

  .hidden {
    display: none;
  }

  .modal-actions {
    display: flex;
    justify-content: flex-end;
    gap: 10px;
    margin-top: 18px;
  }

  .btn.secondary {
    background: rgba(255, 255, 255, 0.08);
  }

  .btn.secondary:hover {
    transform: scale(1.05);
  }

  @media (max-width: 640px) {
    .form-grid {
      grid-template-columns: 1fr;
    }
  }

      /* ================== TOAST ================== */
      .toast {
        position: fixed;
        bottom: 20px;
        right: 20px;
        background: #22c55e;
        padding: 10px 20px;
        border-radius: 10px;
        display: none;
      }

th, td {
    padding: 10px 12px;
    text-align: left;
    border-bottom: 0.2px solid rgba(255,255,255,0.1);
    font-family: "Segoe UI", Arial, sans-serif;
    font-size: 0.9rem;
}

td{
     color: #e5e7eb;               
}
th {
    background-color: #22c55e;     /* لون رأس الجدول أغمق قليلًا من background body */
    // #020617
    // #22c55e
    color: #000000;  !important
    // #38bdf8
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}
    table, th, td {
    border: none;
}
    
/* صفوف بديلة بتدرج غامق */
tr:nth-child(even) {
    background-color: rgba(255,255,255,0.03);
}

table {
    width: 100%;
    border-collapse: collapse;
    margin-top: 12px;
    background-color: #111827;   /* نفس لون خلفية الكارت الداكنة */
    border-radius: 12px;
    overflow: hidden;
    border-collapse: separate;
    border-spacing: 0;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
}

tr:hover {
    background-color: rgba(56, 189, 248, 0.15); /* لون زرقي شفاف عند الـ hover */
    cursor: default;
}

/* ================== SWITCH BADGE ================== */
      .switch-badge {
        background: rgba(56, 189, 248, 0.15);
        color: #38bdf8;
        padding: 4px 10px;
        border-radius: 8px;
        font-size: 0.8rem;
        border: 1px solid rgba(56, 189, 248, 0.3);
        display: inline-block;
        white-space: nowrap;
      }
    </style>
  </head>

  <body>
    <!-- SIDEBAR -->
    <div class="sidebar">
      <h2>Smart Home</h2>
      <ul>
        <li onclick="showDevices()">Devices</li>
        <li onclick="showRooms()">Rooms</li>
        <li onclick="showControllerSources()">Controller Sources</li>
        <li onclick="showControllers()">Controllers</li>
        <li onclick="showProtocols()">Protocols</li>
      </ul>
    </div>

    <!-- MAIN -->
    <div class="main">
      <!-- TOPBAR -->
      <div class="topbar">
        <h2>Devices</h2>
        <div>
          <span class="status">● Connected</span>
          <button class="btn" onclick="openModal()">+ Add Device</button>
        </div>
      </div>)rawliteral");

      getTable();

      server.sendContent(R"rawliteral(
      <!-- GRID -->
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

      /* ================== ADD ================== */
      function addDevice() {
        const name = document.getElementById("name").value.trim();
        const room = document.getElementById("room").value.trim();
        const controllerId = document.getElementById("controllerId").value.trim();
        const protocol = document.getElementById("protocol").value;
        const mac = document.getElementById("mac").value.trim();
        const pin = document.getElementById("pin").value.trim();
const switchPin = document.getElementById("switchPin").value.trim();

        if (!name || !room || !controllerId || !pin||!switchPin) {
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
  
.then(() => {
  closeModal();
  showToast("Device added");
  location.reload();
});
      

      fetch("/api/switch/add", {
  method: "POST",
  headers: {
    "Content-Type": "application/json"
  },
  body: JSON.stringify({
    switchPin: parseInt(switchPin)
  })
})
.then(async (res) => {
  if (!res.ok) {
    const msg = await res.text();
    throw new Error(msg || "Failed to add switch");
  }
  return res.text();
})
.then(() => {
  showToast("Switch added successfully");
  location.reload();
})
.catch((err) => {
  showToast(err.message);
});
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
)rawliteral"); });
 
    server.on("/api/device/add", HTTP_POST, [this]()
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



  server.send(200, "text/plain", "OK"); });

  //////////////////////////////////////////////////
  //////////////////////////////////////////////////

    server.on("/api/device/delete", HTTP_POST, [this]()
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

  server.send(200, "text/plain", "OK"); });

  //////////////////////////////////////////////
  //////////////////////////////////////////////

  server.on("/api/switch/add", HTTP_POST, [this]()
{
  String body = server.arg("plain");

  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, body);

  if (error)
  {
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }

  uint32_t deviceId = nextId-1;
  int switchPin = doc["switchPin"] | -1;

  if (deviceId == 0 || switchPin < 0)
  {
    server.send(400, "text/plain", "Missing or invalid fields");
    return;
  }


  server.send(200, "text/plain", "OK");
});
//////////////////////////////////
//////////////////////////////////

    server.begin();
  }

  void handle()
  {
    server.handleClient();
  }
};