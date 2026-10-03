#pragma once

#include <Arduino.h>
#include <vector>
#include "domain/entities/Room.h"
#include "domain/entities/Controller.h"
#include "domain/entities/protocol/Protocol.h"

struct DiscoveredWirelessPeer
{
    uint8_t mac[6];
    uint8_t channel;
};

struct WirelessDevice
{
    int id;
    uint8_t mac[6];
    int channel;
    bool hasMac;
    int protocolId;
};

class RoomStorage
{
private:
    static std::vector<Room> rooms;
    static int nextId;

public:
    static const std::vector<Room> &getAll();
    static int add(const String &name);
    static bool update(int id, const String &name);
    static bool remove(int id);
    static bool contains(int id);
    static bool isUsed(int id);
    static void restoreAll(const std::vector<Room> &records);
};

class WirelessStorage
{
private:
    static std::vector<DiscoveredWirelessPeer> discovered;
    static std::vector<WirelessDevice> devices;
    static int nextId;

public:
    static const std::vector<DiscoveredWirelessPeer> &getDiscovered();
    static const std::vector<WirelessDevice> &getAll();
    static void recordDiscovery(const uint8_t mac[6], uint8_t channel);
    static bool removeDiscovery(const uint8_t mac[6]);
    static int add(const uint8_t mac[6], uint8_t channel, int protocolId);
    static int addUnassigned(int protocolId);
    static bool update(int id, int protocolId);
    static bool assignMac(int id, const uint8_t mac[6], uint8_t channel);
    static bool remove(int id);
    static const WirelessDevice *find(int id);
    static void restoreAll(const std::vector<WirelessDevice> &records);
};

class ControllerStorage
{
private:
    static std::vector<Controller> controllers;
    static int nextId;

public:
    static const std::vector<Controller> &getAll();
    static int add(int roomId);
    static bool update(int id, int roomId);
    static bool remove(int id);
    static bool contains(int id);
    static bool isUsed(int id);
    static void restoreAll(const std::vector<Controller> &records);
};

class ProtocolStorage
{
private:
    static std::vector<Protocol> protocols;
    static int nextId;

public:
    static const std::vector<Protocol> &getAll();
    static int add(const String &name);
    static int add(const String &name, Protocol::Kind kind);
    static bool update(int id, const String &name);
    static bool update(int id, const String &name, Protocol::Kind kind);
    static bool remove(int id);
    static bool contains(int id);
    static bool isUsed(int id);
    static bool isWirelessUsed(int id);
    static const Protocol *find(int id);
    static void restoreAll(const std::vector<Protocol> &records);
};
