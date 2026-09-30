#pragma once

#include "adapters/api/EspServer.h"
#include "adapters/data_manager/ControllerSourceStorage.h"
#include "adapters/data_manager/EntityStorage.h"
#include "adapters/data_manager/LightStorage.h"

class EntitiesPage
{
private:
    static void beginResponse()
    {
        EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        EspServer::server.send(200, "text/html", "");
    }

    static String escapeHtml(const String &value)
    {
        String escaped;
        for (unsigned int i = 0; i < value.length(); ++i)
        {
            switch (value[i])
            {
            case '&': escaped += "&amp;"; break;
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&#39;"; break;
            default: escaped += value[i]; break;
            }
        }
        return escaped;
    }

    static String roomName(int id)
    {
        for (const Room &room : RoomStorage::getAll())
        {
            if (room.id == id)
                return String(room.name.c_str());
        }
        return "";
    }

    static String protocolName(int id)
    {
        for (const Protocol &protocol : ProtocolStorage::getAll())
        {
            if (protocol.getId() == id)
                return String(protocol.getName().c_str());
        }
        return "";
    }

    static String controllerName(int id)
    {
        for (const Controller &controller : ControllerStorage::getAll())
        {
            if (controller.getId() == id)
                return "Controller " + String(id) + " (Room " + String(controller.getRoomId()) + ")";
        }
        return "";
    }

    static void sendActions(const String &type, int id, const String &fields)
    {
        EspServer::server.sendContent(
            "<td><button class='btn' data-type='" + type +
            "' data-id='" + String(id) + "'" + fields +
            " onclick='editEntity(this)'>Edit</button> "
            "<button class='delete-btn' data-type='" + type +
            "' data-id='" + String(id) +
            "' onclick='deleteEntity(this)'>Delete</button></td>");
    }

    static void showSources(bool buttonsOnly)
    {
        const String title = buttonsOnly ? "Buttons" : "Control Sources";
        const String type = buttonsOnly ? "button" : "source";
        EspServer::server.sendContent("<h3>" + title + "</h3><table><thead><tr>"
                                      "<th>ID</th><th>Pin Number</th><th>Controller ID</th>");
        if (buttonsOnly)
            EspServer::server.sendContent("<th>Button Type</th>");
        EspServer::server.sendContent("<th>Actions</th></tr></thead><tbody>");

        for (const ControllerSource &source : ControllerSourceStorage::getAll())
        {
            if ((source.getButtonType() >= 0) != buttonsOnly)
                continue;
            String controllerLabel = source.getControllerId() == -1
                ? String("Main controller")
                : "Controller " + String(source.getControllerId());
            String fields = " data-pin-number='" + String(source.getPinNumber()) +
                            "' data-controller-id='" + String(source.getControllerId()) +
                            "' data-controller-id-label='" + controllerLabel + "'";
            if (buttonsOnly)
                fields += " data-button-type='" + String(source.getButtonType()) + "'";
            EspServer::server.sendContent("<tr><td>" + String(source.getId()) +
                                          "</td><td>" + String(source.getPinNumber()) +
                                          "</td><td>" + String(source.getControllerId()) + "</td>");
            if (buttonsOnly)
                EspServer::server.sendContent(String("<td>") +
                    (source.getButtonType() == 0 ? "PUSH" : "SWITCH") + "</td>");
            sendActions(type, source.getId(), fields);
            EspServer::server.sendContent("</tr>");
        }
        EspServer::server.sendContent("</tbody></table>");
    }

public:
    static void showDevices()
    {
        beginResponse();
        EspServer::server.sendContent(
            "<h3>Devices</h3><table><thead><tr><th>ID</th><th>Name</th><th>Room</th>"
            "<th>Controller</th><th>Protocol</th><th>Pin Number</th>"
            "<th>Number On Light</th><th>State</th><th>Actions</th></tr></thead><tbody>");
        for (const Light &light : LightStorage::getLights())
        {
            String room = escapeHtml(roomName(light.roomId));
            String controller = escapeHtml(controllerName(light.controllerId));
            String protocol = escapeHtml(protocolName(light.protocolId));
            String fields = " data-name='" + escapeHtml(String(light.name.c_str())) +
                "' data-room-id='" + String(light.roomId) +
                "' data-room-id-label='" + room +
                "' data-controller-id='" + String(light.controllerId) +
                "' data-controller-id-label='" + controller +
                "' data-protocol-id='" + String(light.protocolId) +
                "' data-protocol-id-label='" + protocol +
                "' data-output-number='" + String(light.outputNumber) +
                "' data-number-on-light='" + String(light.numberOnLight) +
                "' data-state='" + String(light.state ? 1 : 0) + "'";
            EspServer::server.sendContent("<tr><td>" + String(light.id) +
                "</td><td>" + escapeHtml(String(light.name.c_str())) +
                "</td><td>" + room +
                "</td><td>" + controller +
                "</td><td>" + protocol +
                "</td><td>" + String(light.outputNumber) +
                "</td><td>" + String(light.numberOnLight) +
                "</td><td>" + String(light.state ? "ON" : "OFF") + "</td>");
            sendActions("device", light.id, fields);
            EspServer::server.sendContent("</tr>");
        }
        EspServer::server.sendContent("</tbody></table>");
    }

    static void showRooms()
    {
        beginResponse();
        EspServer::server.sendContent(
            "<h3>Rooms</h3><table><thead><tr><th>ID</th><th>Name</th><th>Actions</th>"
            "</tr></thead><tbody>");
        for (const Room &room : RoomStorage::getAll())
        {
            String name = escapeHtml(String(room.name.c_str()));
            EspServer::server.sendContent("<tr><td>" + String(room.id) + "</td><td>" +
                                          name + "</td>");
            sendActions("room", room.id, " data-name='" + name + "'");
            EspServer::server.sendContent("</tr>");
        }
        EspServer::server.sendContent("</tbody></table>");
    }

    static void showControllers()
    {
        beginResponse();
        EspServer::server.sendContent(
            "<h3>Controllers</h3><table><thead><tr><th>ID</th><th>Room ID</th>"
            "<th>Actions</th></tr></thead><tbody>");
        for (const Controller &controller : ControllerStorage::getAll())
        {
            String room = escapeHtml(roomName(controller.getRoomId()));
            EspServer::server.sendContent("<tr><td>" + String(controller.getId()) +
                                          "</td><td>" + room + "</td>");
            sendActions("controller", controller.getId(), " data-room-id='" +
                        String(controller.getRoomId()) +
                        "' data-room-id-label='" + room + "'");
            EspServer::server.sendContent("</tr>");
        }
        EspServer::server.sendContent("</tbody></table>");
    }

    static void showProtocols()
    {
        beginResponse();
        EspServer::server.sendContent(
            "<h3>Protocols</h3><table><thead><tr><th>ID</th><th>Name</th><th>Type</th><th>Actions</th>"
            "</tr></thead><tbody>");
        for (const Protocol &protocol : ProtocolStorage::getAll())
        {
            String name = escapeHtml(String(protocol.getName().c_str()));
            String kind = protocol.getKind() == Protocol::Kind::WIRELESS ? "Wireless" : "Wired";
            EspServer::server.sendContent("<tr><td>" + String(protocol.getId()) +
                                          "</td><td>" + name + "</td><td>" + kind + "</td>");
            sendActions("protocol", protocol.getId(), " data-name='" + name +
                        "' data-kind='" +
                        String(protocol.getKind() == Protocol::Kind::WIRELESS ? 1 : 0) + "'");
            EspServer::server.sendContent("</tr>");
        }
        EspServer::server.sendContent("</tbody></table>");
    }

    static void showWireless()
    {
        beginResponse();
        EspServer::server.sendContent(
            "<h3>Wireless</h3><table><thead><tr><th>ID</th><th>MAC</th><th>Channel</th>"
            "<th>Protocol ID</th><th>Details</th><th>Actions</th></tr></thead><tbody>");
        for (const WirelessDevice &device : WirelessStorage::getAll())
        {
            char mac[18];
            snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
                     device.mac[0], device.mac[1], device.mac[2],
                     device.mac[3], device.mac[4], device.mac[5]);
            const Protocol *protocol = ProtocolStorage::find(device.protocolId);
            String protocolNameValue = protocol
                ? escapeHtml(String(protocol->getName().c_str()))
                : String("Missing protocol");
            String fields = " data-protocol-id='" + String(device.protocolId) +
                            "' data-protocol-id-label='" + protocolNameValue + "'";
            EspServer::server.sendContent("<tr><td>" + String(device.id) +
                "</td><td>" + String(mac) +
                "</td><td>" + String(device.channel) +
                "</td><td>" + String(device.protocolId) +
                "</td><td><button class='btn' title='Show protocol details' "
                "onclick='showWirelessDetails(" + String(device.id) +
                ")'>&#9432;</button></td>");
            sendActions("wireless", device.id, fields);
            EspServer::server.sendContent("</tr>");
        }
        EspServer::server.sendContent("</tbody></table>");
    }

    static void showControlSources()
    {
        beginResponse();
        showSources(false);
    }

    static void showButtons()
    {
        beginResponse();
        showSources(true);
    }

    static void sendModal()
    {
        EspServer::server.sendContent(R"rawliteral(
<dialog id="entityModal" class="modal-dialog">
  <div class="modal-content">
    <div class="modal-header">
      <h3 id="entityModalTitle">Add</h3>
      <button type="button" class="close-btn" onclick="closeEntityModal()">&times;</button>
    </div>
    <form id="entityForm">
      <div id="entityFields"></div>
      <div class="modal-actions">
        <button type="button" class="btn secondary" onclick="closeEntityModal()">Cancel</button>
        <button type="submit" class="btn">Save</button>
      </div>
    </form>
  </div>
</dialog>
<dialog id="relationModal" class="modal-dialog">
  <div class="modal-content">
    <div class="modal-header">
      <h3 id="relationModalTitle">Select related item</h3>
      <button type="button" class="close-btn" onclick="closeRelationPicker()">&times;</button>
    </div>
    <div id="relationOptions"></div>
    <div id="newRoomControls" class="form-group mb-14" style="display:none">
      <label for="newRoomName">Or create a new room</label>
      <input id="newRoomName" type="text" placeholder="Room name" />
      <button type="button" class="btn" onclick="createRoomFromPicker()">Create and select</button>
    </div>
    <div id="newProtocolControls" class="form-group mb-14" style="display:none">
      <label for="newProtocolName">Create new protocol</label>
      <input id="newProtocolName" type="text" placeholder="Protocol name" />
      <button type="button" class="btn" onclick="createProtocolFromPicker()">Create and select</button>
    </div>
    <div class="modal-actions">
      <button type="button" class="btn secondary" onclick="closeRelationPicker()">Cancel</button>
      <button type="button" class="btn" onclick="confirmRelationSelection()">OK</button>
    </div>
  </div>
</dialog>
<dialog id="wirelessDiscoveryModal" class="modal-dialog">
  <div class="modal-content">
    <div class="modal-header">
      <h3>Discover wireless devices</h3>
      <button type="button" class="close-btn" onclick="closeWirelessDiscovery()">&times;</button>
    </div>
    <div class="discovery-status">
      <div class="discovery-spinner" id="wirelessSpinner"></div>
      <p id="wirelessDiscoveryStatus">Listening for ESP-NOW broadcasts...</p>
    </div>
    <div id="wirelessDiscoveryResults"></div>
    <div class="modal-actions">
      <button type="button" class="btn secondary" onclick="closeWirelessDiscovery()">Cancel</button>
      <button type="button" class="btn" onclick="continueWirelessSetup()">Next</button>
    </div>
  </div>
</dialog>
<dialog id="wirelessDetailsModal" class="modal-dialog">
  <div class="modal-content">
    <div class="modal-header">
      <h3>Protocol details</h3>
      <button type="button" class="close-btn" onclick="document.getElementById('wirelessDetailsModal').close()">&times;</button>
    </div>
    <div id="wirelessDetailsContent"></div>
  </div>
</dialog>
<div class="toast" id="toast"></div>
)rawliteral");
    }

    static void showOptions()
    {
        String type = EspServer::server.arg("type");
        EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        EspServer::server.send(200, "text/html", "");
        EspServer::server.sendContent("<table><thead><tr><th>Select</th><th>ID</th><th>Name</th></tr></thead><tbody>");

        if (type == "room")
        {
            for (const Room &room : RoomStorage::getAll())
            {
                String label = escapeHtml(String(room.name.c_str()));
                EspServer::server.sendContent("<tr><td><input type='radio' name='relationChoice' value='" +
                    String(room.id) + "' data-label='" + label + "'></td><td>" + String(room.id) +
                    "</td><td>" + label + "</td></tr>");
            }
        }
        else if (type == "controller")
        {
            for (const Controller &controller : ControllerStorage::getAll())
            {
                String label = "Controller " + String(controller.getId()) + " (Room " +
                               String(controller.getRoomId()) + ")";
                EspServer::server.sendContent("<tr><td><input type='radio' name='relationChoice' value='" +
                    String(controller.getId()) + "' data-label='" + label + "'></td><td>" +
                    String(controller.getId()) + "</td><td>" + label + "</td></tr>");
            }
        }
        else if (type == "controllerSource")
        {
            EspServer::server.sendContent("<tr><td><input type='radio' name='relationChoice' value='-1' data-label='Main controller'></td><td>-1</td><td>Main controller</td></tr>");
            for (const Controller &controller : ControllerStorage::getAll())
            {
                String label = "Controller " + String(controller.getId()) + " (Room " +
                               String(controller.getRoomId()) + ")";
                EspServer::server.sendContent("<tr><td><input type='radio' name='relationChoice' value='" +
                    String(controller.getId()) + "' data-label='" + label + "'></td><td>" +
                    String(controller.getId()) + "</td><td>" + label + "</td></tr>");
            }
        }
        else if (type == "protocol" || type == "wirelessProtocol")
        {
            for (const Protocol &protocol : ProtocolStorage::getAll())
            {
                if (type == "wirelessProtocol" &&
                    protocol.getKind() != Protocol::Kind::WIRELESS)
                    continue;
                String label = escapeHtml(String(protocol.getName().c_str()));
                label += protocol.getKind() == Protocol::Kind::WIRELESS
                    ? " (Wireless)" : " (Wired)";
                EspServer::server.sendContent("<tr><td><input type='radio' name='relationChoice' value='" +
                    String(protocol.getId()) + "' data-label='" + label + "'></td><td>" +
                    String(protocol.getId()) + "</td><td>" + label + "</td></tr>");
            }
        }
        else
        {
            EspServer::server.sendContent("<tr><td colspan='3'>Unknown relation type</td></tr>");
        }

        EspServer::server.sendContent("</tbody></table>");
        if (type == "room")
            EspServer::server.sendContent("<p>Select a room above, or create one below.</p>");
        else if (type == "protocol")
            EspServer::server.sendContent("<p>Select an existing protocol or create a new one.</p>");
        else if (type == "wirelessProtocol")
            EspServer::server.sendContent("<p>Select a wireless protocol or create a new one.</p>");
    }

    static void sendWirelessDetails(int id)
    {
        const WirelessDevice *device = WirelessStorage::find(id);
        if (device == NULL)
        {
            EspServer::server.send(404, "text/plain", "Wireless device not found");
            return;
        }
        const Protocol *protocol = ProtocolStorage::find(device->protocolId);
        if (protocol == NULL)
        {
            EspServer::server.send(404, "text/plain", "Assigned protocol not found");
            return;
        }
        char mac[18];
        snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
                 device->mac[0], device->mac[1], device->mac[2],
                 device->mac[3], device->mac[4], device->mac[5]);
        String kind = protocol->getKind() == Protocol::Kind::WIRELESS ? "Wireless" : "Wired";
        String html = "<table><tbody><tr><th>Protocol ID</th><td>" +
            String(protocol->getId()) + "</td></tr><tr><th>Name</th><td>" +
            escapeHtml(String(protocol->getName().c_str())) +
            "</td></tr><tr><th>Type</th><td>" + kind +
            "</td></tr><tr><th>Wireless Device ID</th><td>" + String(device->id) +
            "</td></tr><tr><th>MAC</th><td>" + String(mac) +
            "</td></tr><tr><th>Channel</th><td>" + String(device->channel) +
            "</td></tr></tbody></table>";
        EspServer::server.send(200, "text/html", html);
    }
};
