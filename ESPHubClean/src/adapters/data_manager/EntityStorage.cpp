#include <Arduino.h>
#include "EntityStorage.h"
#include "LightStorage.h"
#include "ControllerSourceStorage.h"

std::vector<Room> RoomStorage::rooms;
int RoomStorage::nextId = 0;
std::vector<Controller> ControllerStorage::controllers;
int ControllerStorage::nextId = 0;
std::vector<Protocol> ProtocolStorage::protocols;
int ProtocolStorage::nextId = 0;
std::vector<DiscoveredWirelessPeer> WirelessStorage::discovered;
std::vector<WirelessDevice> WirelessStorage::devices;
int WirelessStorage::nextId = 0;

const std::vector<Room> &RoomStorage::getAll()
{
    return rooms;
}

int RoomStorage::add(const String &name)
{
    Room room;
    room.id = ++nextId;
    room.name = name.c_str();
    rooms.push_back(room);
    return room.id;
}

bool RoomStorage::update(int id, const String &name)
{
    for (Room &room : rooms)
    {
        if (room.id == id)
        {
            room.name = name.c_str();
            LightStorage::updateRoomName(id, room.name);
            return true;
        }
    }
    return false;
}

bool RoomStorage::remove(int id)
{
    if (isUsed(id))
        return false;

    for (std::vector<Room>::iterator it = rooms.begin(); it != rooms.end(); ++it)
    {
        if (it->id == id)
        {
            rooms.erase(it);
            return true;
        }
    }
    return false;
}

bool RoomStorage::contains(int id)
{
    for (const Room &room : rooms)
    {
        if (room.id == id)
            return true;
    }
    return false;
}

bool RoomStorage::isUsed(int id)
{
    for (const Light &light : LightStorage::getLights())
    {
        if (light.roomId == id)
            return true;
    }
    for (const Controller &controller : ControllerStorage::getAll())
    {
        if (controller.getRoomId() == id)
            return true;
    }
    return false;
}

const std::vector<Controller> &ControllerStorage::getAll()
{
    return controllers;
}

int ControllerStorage::add(int roomId)
{
    if (!RoomStorage::contains(roomId))
        return 0;
    int id = ++nextId;
    controllers.push_back(Controller(id, roomId));
    return id;
}

bool ControllerStorage::update(int id, int roomId)
{
    if (!RoomStorage::contains(roomId))
        return false;
    for (Controller &controller : controllers)
    {
        if (controller.getId() == id)
        {
            controller.setRoomId(roomId);
            return true;
        }
    }
    return false;
}

bool ControllerStorage::remove(int id)
{
    if (isUsed(id))
        return false;

    for (std::vector<Controller>::iterator it = controllers.begin(); it != controllers.end(); ++it)
    {
        if (it->getId() == id)
        {
            controllers.erase(it);
            return true;
        }
    }
    return false;
}

bool ControllerStorage::contains(int id)
{
    for (const Controller &controller : controllers)
    {
        if (controller.getId() == id)
            return true;
    }
    return false;
}

bool ControllerStorage::isUsed(int id)
{
    for (const Light &light : LightStorage::getLights())
    {
        if (light.controllerId == id)
            return true;
    }
    for (const ControllerSource &source : ControllerSourceStorage::getAll())
    {
        if (source.getControllerId() == id)
            return true;
    }
    return false;
}

const std::vector<Protocol> &ProtocolStorage::getAll()
{
    return protocols;
}

int ProtocolStorage::add(const String &name, Protocol::Kind kind)
{
    int id = ++nextId;
    protocols.push_back(Protocol(id, std::string(name.c_str()), kind));
    return id;
}

int ProtocolStorage::add(const String &name)
{
    return add(name, Protocol::Kind::WIRED);
}

bool ProtocolStorage::update(int id, const String &name, Protocol::Kind kind)
{
    for (Protocol &protocol : protocols)
    {
        if (protocol.getId() == id)
        {
            protocol.setName(std::string(name.c_str()));
            protocol.setKind(kind);
            return true;
        }
    }
    return false;
}

bool ProtocolStorage::update(int id, const String &name)
{
    const Protocol *protocol = find(id);
    return protocol != NULL && update(id, name, protocol->getKind());
}

bool ProtocolStorage::remove(int id)
{
    if (isUsed(id))
        return false;

    for (std::vector<Protocol>::iterator it = protocols.begin(); it != protocols.end(); ++it)
    {
        if (it->getId() == id)
        {
            protocols.erase(it);
            return true;
        }
    }
    return false;
}

bool ProtocolStorage::contains(int id)
{
    for (const Protocol &protocol : protocols)
    {
        if (protocol.getId() == id)
            return true;
    }
    return false;
}

bool ProtocolStorage::isUsed(int id)
{
    for (const Light &light : LightStorage::getLights())
    {
        if (light.protocolId == id)
            return true;
    }
    for (const WirelessDevice &device : WirelessStorage::getAll())
    {
        if (device.protocolId == id)
            return true;
    }
    return false;
}

bool ProtocolStorage::isWirelessUsed(int id)
{
    for (const WirelessDevice &device : WirelessStorage::getAll())
    {
        if (device.protocolId == id)
            return true;
    }
    return false;
}

const Protocol *ProtocolStorage::find(int id)
{
    for (const Protocol &protocol : protocols)
    {
        if (protocol.getId() == id)
            return &protocol;
    }
    return NULL;
}

const std::vector<DiscoveredWirelessPeer> &WirelessStorage::getDiscovered()
{
    return discovered;
}

const std::vector<WirelessDevice> &WirelessStorage::getAll()
{
    return devices;
}

void WirelessStorage::recordDiscovery(const uint8_t mac[6], uint8_t channel)
{
    for (const WirelessDevice &device : devices)
    {
        if (device.hasMac && memcmp(device.mac, mac, 6) == 0)
            return;
    }
    for (const DiscoveredWirelessPeer &peer : discovered)
    {
        if (memcmp(peer.mac, mac, 6) == 0)
            return;
    }
    if (discovered.size() >= 16)
        discovered.erase(discovered.begin());

    DiscoveredWirelessPeer peer = {};
    memcpy(peer.mac, mac, 6);
    peer.channel = channel;
    discovered.push_back(peer);
}

bool WirelessStorage::removeDiscovery(const uint8_t mac[6])
{
    for (std::vector<DiscoveredWirelessPeer>::iterator it = discovered.begin();
         it != discovered.end(); ++it)
    {
        if (memcmp(it->mac, mac, 6) == 0)
        {
            discovered.erase(it);
            return true;
        }
    }
    return false;
}

int WirelessStorage::add(const uint8_t mac[6], uint8_t channel, int protocolId)
{
    if (!ProtocolStorage::contains(protocolId))
        return 0;
    if (ProtocolStorage::find(protocolId)->getKind() != Protocol::Kind::WIRELESS)
        return 0;
    for (const WirelessDevice &device : devices)
    {
        if (device.hasMac && memcmp(device.mac, mac, 6) == 0)
            return 0;
    }

    WirelessDevice device = {};
    device.id = ++nextId;
    memcpy(device.mac, mac, 6);
    device.channel = channel;
    device.hasMac = true;
    device.protocolId = protocolId;
    devices.push_back(device);
    removeDiscovery(mac);
    return device.id;
}

int WirelessStorage::addUnassigned(int protocolId)
{
    if (!ProtocolStorage::contains(protocolId) ||
        ProtocolStorage::find(protocolId)->getKind() != Protocol::Kind::WIRELESS)
        return 0;

    WirelessDevice device = {};
    device.id = ++nextId;
    device.channel = -1;
    device.hasMac = false;
    device.protocolId = protocolId;
    devices.push_back(device);
    return device.id;
}

bool WirelessStorage::update(int id, int protocolId)
{
    if (!ProtocolStorage::contains(protocolId))
        return false;
    if (ProtocolStorage::find(protocolId)->getKind() != Protocol::Kind::WIRELESS)
        return false;
    for (WirelessDevice &device : devices)
    {
        if (device.id == id)
        {
            device.protocolId = protocolId;
            return true;
        }
    }
    return false;
}

bool WirelessStorage::assignMac(int id, const uint8_t mac[6], uint8_t channel)
{
    for (const WirelessDevice &existing : devices)
    {
        if (existing.id != id && existing.hasMac && memcmp(existing.mac, mac, 6) == 0)
            return false;
    }

    for (WirelessDevice &device : devices)
    {
        if (device.id == id)
        {
            memcpy(device.mac, mac, 6);
            device.channel = channel;
            device.hasMac = true;
            removeDiscovery(mac);
            return true;
        }
    }
    return false;
}

bool WirelessStorage::remove(int id)
{
    for (std::vector<WirelessDevice>::iterator it = devices.begin();
         it != devices.end(); ++it)
    {
        if (it->id == id)
        {
            devices.erase(it);
            return true;
        }
    }
    return false;
}

const WirelessDevice *WirelessStorage::find(int id)
{
    for (const WirelessDevice &device : devices)
    {
        if (device.id == id)
            return &device;
    }
    return NULL;
}
