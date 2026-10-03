#pragma once

#include <stdlib.h>
#include "EspServer.h"
#include "adapters/data_manager/ControllerSourceStorage.h"
#include "adapters/data_manager/EntityStorage.h"
#include "adapters/data_manager/LightStorage.h"
#include "adapters/espnow/WirelessDiscovery.h"
#include "adapters/web/html/EntitiesPage.h"
#include "infrastructure/repository/EntitiesRepository.h"
#include "WebAuth.h"
#include <functional>

class EntitiesApi
{
private:
    template <typename Handler>
    static std::function<void(void)> requireLogin(Handler handler)
    {
        return [handler]()
        {
            if (!WebAuth::isAuthenticated())
            {
                WebAuth::rejectRequest();
                return;
            }
            handler();
        };
    }

    static bool readInteger(const String &name, int &value)
    {
        if (!EspServer::server.hasArg(name) || EspServer::server.arg(name).length() == 0)
            return false;

        String input = EspServer::server.arg(name);
        char *end = NULL;
        long parsed = strtol(input.c_str(), &end, 10);
        if (end == input.c_str() || *end != '\0' || parsed < -2147483647L - 1 || parsed > 2147483647L)
            return false;
        value = static_cast<int>(parsed);
        return true;
    }

    static bool readName(String &name)
    {
        if (!EspServer::server.hasArg("name"))
            return false;
        name = EspServer::server.arg("name");
        name.trim();
        return name.length() > 0;
    }

    static bool readProtocolKind(Protocol::Kind &kind)
    {
        int rawKind;
        if (!readInteger("kind", rawKind) || rawKind < 0 || rawKind > 1)
            return false;
        kind = rawKind == 1 ? Protocol::Kind::WIRELESS : Protocol::Kind::WIRED;
        return true;
    }

    static String formatMac(const uint8_t mac[6])
    {
        char formatted[18];
        snprintf(formatted, sizeof(formatted), "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return String(formatted);
    }

    static bool readSource(const String &type, ControllerSource &source)
    {
        int pinNumber;
        int controllerId;
        if (!readInteger("pinNumber", pinNumber) ||
            !readInteger("controllerId", controllerId) ||
            pinNumber < 0 || controllerId < -1 ||
            (controllerId != -1 && !ControllerStorage::contains(controllerId)))
            return false;

        source.setPinNumber(pinNumber);
        source.setControllerId(controllerId);
        if (type == "button")
        {
            int buttonType;
            if (!readInteger("buttonType", buttonType) || buttonType < 0 || buttonType > 1)
                return false;
            source.setButtonType(buttonType);
        }
        else
        {
            source.setButtonType(-1);
        }
        return true;
    }

    static bool readDevice(Light &light, int id, bool state)
    {
        if (!EspServer::server.hasArg("name"))
            return false;
        int controllerId;
        int protocolId;
        int outputNumber;
        int roomId;
        int numberOnLight;
        if (!readInteger("controllerId", controllerId) ||
            !readInteger("protocolId", protocolId) ||
            !readInteger("outputNumber", outputNumber) ||
            !readInteger("roomId", roomId) ||
            !readInteger("numberOnLight", numberOnLight) ||
            controllerId < 1 || protocolId < 1 || outputNumber < 0 ||
            roomId < 1 || numberOnLight < 0 ||
            !ControllerStorage::contains(controllerId) ||
            !ProtocolStorage::contains(protocolId) ||
            !RoomStorage::contains(roomId))
            return false;

        String name = EspServer::server.arg("name");
        name.trim();
        if (name.length() == 0)
            return false;
        String room;
        for (const Room &storedRoom : RoomStorage::getAll())
        {
            if (storedRoom.id == roomId)
            {
                room = String(storedRoom.name.c_str());
                break;
            }
        }
        light = Light(id, controllerId, std::string(name.c_str()),
                      std::string(room.c_str()), outputNumber, protocolId,
                      roomId, numberOnLight, state);
        return true;
    }

    static void add(const String &type)
    {
        if (type == "room" || type == "protocol")
        {
            String name;
            if (!readName(name))
            {
                EspServer::server.send(400, "text/plain", "A name is required");
                return;
            }
            int id;
            if (type == "room")
                id = RoomStorage::add(name);
            else
            {
                Protocol::Kind kind;
                if (!readProtocolKind(kind))
                {
                    EspServer::server.send(400, "text/plain", "A valid protocol type is required");
                    return;
                }
                id = ProtocolStorage::add(name, kind);
            }
            EspServer::server.send(200, "text/plain", String(id));
            return;
        }
        if (type == "controller")
        {
            int roomId;
            if (!readInteger("roomId", roomId) || roomId < 1 ||
                !RoomStorage::contains(roomId))
            {
                EspServer::server.send(400, "text/plain", "Select an existing room");
                return;
            }
            EspServer::server.send(200, "text/plain", String(ControllerStorage::add(roomId)));
            return;
        }
        if (type == "source" || type == "button")
        {
            if (type == "button")
            {
                int buttonType;
                int sourceId;
                if (!readInteger("buttonType", buttonType) ||
                    buttonType < 0 || buttonType > 1 ||
                    !readInteger("controllerSourceId", sourceId) ||
                    !ControllerSourceStorage::isControlSource(sourceId))
                {
                    EspServer::server.send(400, "text/plain", "Select a valid control source and button type");
                    return;
                }
                ButtonSource button;
                button.setType(static_cast<ButtonSource::Type>(buttonType));
                button.setControllerSourceId(sourceId);
                int id = ControllerSourceStorage::addButton(button);
                if (id == 0)
                {
                    EspServer::server.send(500, "text/plain", "Could not add button");
                    return;
                }
                EspServer::server.send(200, "text/plain", String(id));
                return;
            }
            ControllerSource source;
            if (!readSource(type, source))
            {
                EspServer::server.send(400, "text/plain", "Invalid pin, controller, or button type");
                return;
            }
            if (!ControllerSourceStorage::add(source))
            {
                EspServer::server.send(500, "text/plain", "Could not add source");
                return;
            }
            EspServer::server.send(200, "text/plain", String(ControllerSourceStorage::getAll().back().getId()));
            return;
        }
        if (type == "device")
        {
            Light light(0, 0, "", "", 0, 0, 0, 0);
            if (!readDevice(light, 0, false))
            {
                EspServer::server.send(400, "text/plain", "Invalid device fields");
                return;
            }
            EspServer::server.send(200, "text/plain", String(LightStorage::addLight(light)));
            return;
        }
        if (type == "wireless")
        {
            int protocolId;
            if (!EspServer::server.hasArg("mac"))
            {
                EspServer::server.send(400, "text/plain", "MAC is required; use -1 to assign it later");
                return;
            }
            String macText = EspServer::server.arg("mac");
            bool createProtocol = EspServer::server.hasArg("protocolName");
            String protocolName;
            if (createProtocol)
            {
                protocolName = EspServer::server.arg("protocolName");
                protocolName.trim();
                if (protocolName.length() == 0)
                {
                    EspServer::server.send(400, "text/plain", "Protocol name is required");
                    return;
                }
            }
            else if (!readInteger("protocolId", protocolId) ||
                     !ProtocolStorage::contains(protocolId) ||
                     ProtocolStorage::find(protocolId)->getKind() != Protocol::Kind::WIRELESS)
            {
                EspServer::server.send(400, "text/plain", "Select an existing wireless protocol");
                return;
            }

            if (macText == "-1")
            {
                if (createProtocol)
                    protocolId = ProtocolStorage::add(protocolName, Protocol::Kind::WIRELESS);
                int id = WirelessStorage::addUnassigned(protocolId);
                if (id == 0)
                {
                    EspServer::server.send(500, "text/plain", "Could not add wireless record");
                    return;
                }
                EspServer::server.send(200, "text/plain", String(id));
                return;
            }

            for (const DiscoveredWirelessPeer &peer : WirelessStorage::getDiscovered())
            {
                if (formatMac(peer.mac) == macText)
                {
                    if (createProtocol)
                        protocolId = ProtocolStorage::add(protocolName, Protocol::Kind::WIRELESS);
                    int id = WirelessStorage::add(peer.mac, peer.channel, protocolId);
                    if (id == 0)
                    {
                        EspServer::server.send(409, "text/plain", "Wireless MAC is already registered");
                        return;
                    }
                    EspServer::server.send(200, "text/plain", String(id));
                    return;
                }
            }
            EspServer::server.send(400, "text/plain", "MAC is no longer in the discovery list; scan again");
            return;
        }
        EspServer::server.send(400, "text/plain", "Unknown entity type");
    }

    static void update(const String &type)
    {
        int id;
        if (!readInteger("id", id) || id < 1)
        {
            EspServer::server.send(400, "text/plain", "A valid id is required");
            return;
        }

        bool updated = false;
        if (type == "room" || type == "protocol")
        {
            String name;
            if (!readName(name))
            {
                EspServer::server.send(400, "text/plain", "A name is required");
                return;
            }
            if (type == "room")
                updated = RoomStorage::update(id, name);
            else
            {
                Protocol::Kind kind;
                if (!readProtocolKind(kind))
                {
                    EspServer::server.send(400, "text/plain", "A valid protocol type is required");
                    return;
                }
                if (kind != Protocol::Kind::WIRELESS &&
                    ProtocolStorage::isWirelessUsed(id))
                {
                    EspServer::server.send(409, "text/plain", "Protocol is assigned to a wireless device");
                    return;
                }
                updated = ProtocolStorage::update(id, name, kind);
            }
        }
        else if (type == "controller")
        {
            int roomId;
            if (!readInteger("roomId", roomId) || roomId < 1 ||
                !RoomStorage::contains(roomId))
            {
                EspServer::server.send(400, "text/plain", "Select an existing room");
                return;
            }
            updated = ControllerStorage::update(id, roomId);
        }
        else if (type == "button")
        {
            int buttonType;
            int sourceId;
            if (!readInteger("buttonType", buttonType) ||
                buttonType < 0 || buttonType > 1 ||
                !readInteger("controllerSourceId", sourceId) ||
                !ControllerSourceStorage::isControlSource(sourceId))
            {
                EspServer::server.send(400, "text/plain", "Select a valid control source and button type");
                return;
            }
            ButtonSource button;
            button.setId(id);
            button.setType(static_cast<ButtonSource::Type>(buttonType));
            button.setControllerSourceId(sourceId);
            updated = ControllerSourceStorage::updateButton(button);
        }
        else if (type == "source")
        {
            ControllerSource source;
            if (!readSource(type, source))
            {
                EspServer::server.send(400, "text/plain", "Invalid pin, controller, or button type");
                return;
            }
            source.setId(id);
            for (const ControllerSource &existing : ControllerSourceStorage::getAll())
            {
                if (existing.getId() == id && existing.getButtonType() < 0)
                {
                    updated = ControllerSourceStorage::update(source);
                    break;
                }
            }
        }
        else if (type == "device")
        {
            Light light(0, 0, "", "", 0, 0, 0, 0);
            int state = 0;
            if (EspServer::server.hasArg("state") && !readInteger("state", state))
            {
                EspServer::server.send(400, "text/plain", "Invalid state");
                return;
            }
            if (state < 0 || state > 1 || !readDevice(light, id, state == 1))
            {
                EspServer::server.send(400, "text/plain", "Invalid device fields");
                return;
            }
            updated = LightStorage::updateLight(light);
        }
        else if (type == "wireless")
        {
            int protocolId;
            if (!readInteger("protocolId", protocolId) || !ProtocolStorage::contains(protocolId))
            {
                EspServer::server.send(400, "text/plain", "Select an existing protocol");
                return;
            }
            if (ProtocolStorage::find(protocolId)->getKind() != Protocol::Kind::WIRELESS)
            {
                EspServer::server.send(400, "text/plain", "Select a wireless protocol");
                return;
            }
            updated = WirelessStorage::update(id, protocolId);
        }
        else
        {
            EspServer::server.send(400, "text/plain", "Unknown entity type");
            return;
        }

        if (!updated)
        {
            EspServer::server.send(404, "text/plain", "Entity not found");
            return;
        }
        EspServer::server.send(200, "text/plain", "Updated successfully");
    }

    static void remove(const String &type)
    {
        int id;
        if (!readInteger("id", id) || id < 1)
        {
            EspServer::server.send(400, "text/plain", "A valid id is required");
            return;
        }

        if (type == "room" && RoomStorage::isUsed(id))
        {
            EspServer::server.send(409, "text/plain", "Room is used by a device or controller");
            return;
        }
        if (type == "controller" && ControllerStorage::isUsed(id))
        {
            EspServer::server.send(409, "text/plain", "Controller is used by a device or control source");
            return;
        }
        if (type == "protocol" && ProtocolStorage::isUsed(id))
        {
            EspServer::server.send(409, "text/plain", "Protocol is used by a device or wireless record");
            return;
        }

        bool removed = false;
        if (type == "room")
            removed = RoomStorage::remove(id);
        else if (type == "controller")
            removed = ControllerStorage::remove(id);
        else if (type == "protocol")
            removed = ProtocolStorage::remove(id);
        else if (type == "device")
            removed = LightStorage::removeLight(id);
        else if (type == "wireless")
            removed = WirelessStorage::remove(id);
        else if (type == "button")
        {
            removed = ControllerSourceStorage::removeButton(id);
        }
        else if (type == "source")
        {
            if (LightControllerSourceStorage::isControlSourceAssociatedWithLight(id) ||
                ControllerSourceStorage::isControlSourceUsed(id))
            {
                EspServer::server.send(409, "text/plain", "Control source is assigned to a device or button");
                return;
            }
            for (const ControllerSource &source : ControllerSourceStorage::getAll())
            {
                if (source.getId() == id && source.getButtonType() < 0)
                {
                    removed = ControllerSourceStorage::remove(id);
                    break;
                }
            }
        }
        else
        {
            EspServer::server.send(400, "text/plain", "Unknown entity type");
            return;
        }

        if (!removed)
        {
            EspServer::server.send(404, "text/plain", "Entity not found");
            return;
        }
        EspServer::server.send(200, "text/plain", "Deleted successfully");
    }

    static String requestType()
    {
        return EspServer::server.arg("type");
    }

    static void addEntity()
    {
        add(requestType());
    }

    static void updateEntity()
    {
        update(requestType());
    }

    static void deleteEntity()
    {
        remove(requestType());
    }

    static void addControlSource()
    {
        add("source");
    }

    static void updateControlSource()
    {
        update("source");
    }

    static void deleteControlSource()
    {
        remove("source");
    }

    static void showAddModal()
    {
        EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        EspServer::server.send(200, "text/html", "");
        EntitiesPage::sendModal();
    }

    static void showOptions()
    {
        EntitiesPage::showOptions();
    }

    static void getWirelessDiscovery()
    {
        uint8_t mac[6];
        uint8_t channel;
        if (WirelessDiscovery::take(mac, channel))
            WirelessStorage::recordDiscovery(mac, channel);

        String body = "[";
        const std::vector<DiscoveredWirelessPeer> &peers = WirelessStorage::getDiscovered();
        for (size_t i = 0; i < peers.size(); ++i)
        {
            if (i > 0)
                body += ",";
            body += "{\"mac\":\"" + formatMac(peers[i].mac) +
                    "\",\"channel\":" + String(peers[i].channel) + "}";
        }
        body += "]";
        EspServer::server.send(200, "application/json", body);
    }

    static void addWireless()
    {
        add("wireless");
    }

    static void showWirelessDetails()
    {
        int id;
        if (!readInteger("id", id))
        {
            EspServer::server.send(400, "text/plain", "A valid id is required");
            return;
        }
        EntitiesPage::sendWirelessDetails(id);
    }

    static void showControlSourceDetails()
    {
        int id;
        if (!readInteger("id", id))
        {
            EspServer::server.send(400, "text/plain", "A valid id is required");
            return;
        }
        EntitiesPage::sendControlSourceDetails(id);
    }

    static void showDeviceDetails()
    {
        int id;
        if (!readInteger("id", id) || id < 1)
        {
            EspServer::server.send(400, "text/plain", "A valid id is required");
            return;
        }
        EntitiesPage::sendDeviceDetails(id);
    }

    static void showControllerOptions()
    {
        EntitiesPage::showControllerOptions();
    }

    static void saveData()
    {
        String scope = EspServer::server.arg("scope");
        if (!EspServer::server.hasArg("scope") || scope.isEmpty())
        {
            EspServer::server.send(400, "text/plain", "A save scope is required");
            return;
        }
        if (scope != "all" && scope != "Rooms" && scope != "Devices" &&
            scope != "Controllers" && scope != "Protocols" &&
            scope != "Wireless" && scope != "Control Sources" &&
            scope != "Buttons")
        {
            EspServer::server.send(400, "text/plain", "Unknown data category");
            return;
        }
        String error;
        bool saved = scope == "all"
            ? EntitiesRepository::saveAll(error)
            : EntitiesRepository::saveCategory(scope, error);
        if (!saved)
        {
            EspServer::server.send(500, "text/plain", error);
            return;
        }
        EspServer::server.send(200, "text/plain",
                               scope == "all" ? "All application data saved to ESP"
                                              : scope + " data saved to ESP");
    }

    static void assignWirelessMac()
    {
        int id;
        if (!readInteger("id", id) || id < 1 || !EspServer::server.hasArg("mac"))
        {
            EspServer::server.send(400, "text/plain", "A valid device id and discovered MAC are required");
            return;
        }
        String macText = EspServer::server.arg("mac");
        for (const DiscoveredWirelessPeer &peer : WirelessStorage::getDiscovered())
        {
            if (formatMac(peer.mac) == macText)
            {
                if (!WirelessStorage::assignMac(id, peer.mac, peer.channel))
                {
                    EspServer::server.send(409, "text/plain", "MAC is already assigned or device was not found");
                    return;
                }
                EspServer::server.send(200, "text/plain", "MAC assigned successfully");
                return;
            }
        }
        EspServer::server.send(400, "text/plain", "MAC is no longer in the discovery list; scan again");
    }

public:
    static void begin()
    {
        EspServer::server.on("/devices/show", HTTP_GET, requireLogin(EntitiesPage::showDevices));
        EspServer::server.on("/showDevices", HTTP_GET, requireLogin(EntitiesPage::showDevices));
        EspServer::server.on("/showRooms", HTTP_GET, requireLogin(EntitiesPage::showRooms));
        EspServer::server.on("/showRoomDiagram", HTTP_GET, requireLogin(EntitiesPage::showRoomDiagram));
        EspServer::server.on("/showControllers", HTTP_GET, requireLogin(EntitiesPage::showControllers));
        EspServer::server.on("/showProtocols", HTTP_GET, requireLogin(EntitiesPage::showProtocols));
        EspServer::server.on("/showControlSources", HTTP_GET, requireLogin(EntitiesPage::showControlSources));
        EspServer::server.on("/showButtons", HTTP_GET, requireLogin(EntitiesPage::showButtons));
        EspServer::server.on("/showWireless", HTTP_GET, requireLogin(EntitiesPage::showWireless));

        EspServer::server.on("/api/entities/add", HTTP_POST, requireLogin(addEntity));
        EspServer::server.on("/api/entities/update", HTTP_POST, requireLogin(updateEntity));
        EspServer::server.on("/api/entities/delete", HTTP_POST, requireLogin(deleteEntity));
        EspServer::server.on("/api/entities/options", HTTP_GET, requireLogin(showOptions));
        EspServer::server.on("/api/entities/controller-options", HTTP_GET, requireLogin(showControllerOptions));
        EspServer::server.on("/api/storage/save", HTTP_POST, requireLogin(saveData));
        EspServer::server.on("/api/wireless/discovery", HTTP_GET, requireLogin(getWirelessDiscovery));
        EspServer::server.on("/api/wireless/add", HTTP_POST, requireLogin(addWireless));
        EspServer::server.on("/api/wireless/assign", HTTP_POST, requireLogin(assignWirelessMac));
        EspServer::server.on("/api/wireless/details", HTTP_GET, requireLogin(showWirelessDetails));
        EspServer::server.on("/api/control-sources/details", HTTP_GET, requireLogin(showControlSourceDetails));
        EspServer::server.on("/api/devices/details", HTTP_GET, requireLogin(showDeviceDetails));
        EspServer::server.on("/api/wireless/update", HTTP_POST, requireLogin(updateEntity));
        EspServer::server.on("/api/wireless/delete", HTTP_POST, requireLogin(deleteEntity));

        EspServer::server.on("/ControlSources/add", HTTP_POST, requireLogin(addControlSource));
        EspServer::server.on("/ControlSources/update", HTTP_POST, requireLogin(updateControlSource));
        EspServer::server.on("/ControlSources/delete", HTTP_DELETE, requireLogin(deleteControlSource));
        EspServer::server.on("/ControlSources/showAdd", HTTP_GET, requireLogin(showAddModal));
    }
};
