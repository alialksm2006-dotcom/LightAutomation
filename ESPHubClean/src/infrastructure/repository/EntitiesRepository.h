#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <vector>

#include "adapters/data_manager/ControllerSourceStorage.h"
#include "adapters/data_manager/EntityStorage.h"
#include "adapters/data_manager/LightStorage.h"

class EntitiesRepository
{
private:
    static bool writeDocument(const char *nameSpace, JsonDocument &document, String &error)
    {
        String payload;
        if (serializeJson(document, payload) == 0)
        {
            error = "Could not serialize " + String(nameSpace) + " data";
            return false;
        }

        Preferences preferences;
        if (!preferences.begin(nameSpace, false))
        {
            error = "Could not open ESP storage for " + String(nameSpace);
            return false;
        }
        const size_t written = preferences.putString("data", payload);
        preferences.end();
        if (written != payload.length())
        {
            error = "Could not write all " + String(nameSpace) + " data to ESP storage";
            return false;
        }
        return true;
    }

    static bool readDocument(const char *nameSpace, JsonDocument &document,
                             bool &found, String &error)
    {
        Preferences preferences;
        if (!preferences.begin(nameSpace, true))
        {
            error = "Could not open ESP storage for " + String(nameSpace);
            return false;
        }
        found = preferences.isKey("data");
        if (!found)
        {
            preferences.end();
            return true;
        }

        String payload = preferences.getString("data", "");
        preferences.end();
        if (payload.isEmpty())
        {
            error = "Stored " + String(nameSpace) + " data is empty";
            return false;
        }
        DeserializationError result = deserializeJson(document, payload);
        if (result || !document.is<JsonArray>())
        {
            error = "Stored " + String(nameSpace) + " data is invalid";
            return false;
        }
        return true;
    }

    static bool saveRooms(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const Room &room : RoomStorage::getAll())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = room.id;
            record["name"] = room.name;
        }
        return writeDocument("rooms", document, error);
    }

    static bool saveDevices(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const Light &light : LightStorage::getLights())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = light.id;
            record["controllerId"] = light.controllerId;
            record["name"] = light.name;
            record["room"] = light.room;
            record["outputNumber"] = light.outputNumber;
            record["protocolId"] = light.protocolId;
            record["roomId"] = light.roomId;
            record["numberOnLight"] = light.numberOnLight;
            record["state"] = light.state;
        }
        return writeDocument("devices", document, error);
    }

    static bool saveControllers(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const Controller &controller : ControllerStorage::getAll())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = controller.getId();
            record["roomId"] = controller.getRoomId();
        }
        return writeDocument("controllers", document, error);
    }

    static bool saveProtocols(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const Protocol &protocol : ProtocolStorage::getAll())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = protocol.getId();
            record["name"] = protocol.getName();
            record["kind"] = static_cast<int>(protocol.getKind());
        }
        return writeDocument("protocols", document, error);
    }

    static bool saveWireless(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const WirelessDevice &device : WirelessStorage::getAll())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = device.id;
            record["hasMac"] = device.hasMac;
            record["channel"] = device.channel;
            record["protocolId"] = device.protocolId;
            JsonArray mac = record["mac"].to<JsonArray>();
            for (size_t i = 0; i < 6; ++i)
                mac.add(device.mac[i]);
        }
        return writeDocument("wireless", document, error);
    }

    static bool saveSources(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const ControllerSource &source : ControllerSourceStorage::getAll())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = source.getId();
            record["pinNumber"] = source.getPinNumber();
            record["controllerId"] = source.getControllerId();
            record["buttonType"] = source.getButtonType();
        }
        return writeDocument("sources", document, error);
    }

    static bool saveButtons(String &error)
    {
        JsonDocument document;
        JsonArray records = document.to<JsonArray>();
        for (const ButtonSource &button : ControllerSourceStorage::getButtons())
        {
            JsonObject record = records.add<JsonObject>();
            record["id"] = button.getId();
            record["type"] = static_cast<int>(button.getType());
            record["controllerSourceId"] = button.getControllerSourceId();
        }
        return writeDocument("buttons", document, error);
    }

    static bool loadRooms(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("rooms", document, found, error) || !found)
            return error.isEmpty();
        std::vector<Room> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            if (!value["id"].is<int>() || !value["name"].is<const char *>())
            {
                error = "Stored room record is invalid";
                return false;
            }
            Room room;
            room.id = value["id"].as<int>();
            room.name = value["name"].as<const char *>();
            if (room.id < 1 || room.name.empty())
            {
                error = "Stored room record is invalid";
                return false;
            }
            records.push_back(room);
        }
        RoomStorage::restoreAll(records);
        return true;
    }

    static bool loadDevices(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("devices", document, found, error) || !found)
            return error.isEmpty();
        std::vector<Light> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            if (!value["id"].is<int>() || !value["controllerId"].is<int>() ||
                !value["name"].is<const char *>() || !value["room"].is<const char *>() ||
                !value["outputNumber"].is<int>() || !value["protocolId"].is<int>() ||
                !value["roomId"].is<int>() || !value["numberOnLight"].is<int>() ||
                !value["state"].is<bool>())
            {
                error = "Stored device record is invalid";
                return false;
            }
            const int id = value["id"].as<int>();
            const int controllerId = value["controllerId"].as<int>();
            const int outputNumber = value["outputNumber"].as<int>();
            const int protocolId = value["protocolId"].as<int>();
            const int roomId = value["roomId"].as<int>();
            const int numberOnLight = value["numberOnLight"].as<int>();
            if (id < 1 || outputNumber < 0 || protocolId < 1 ||
                roomId < 1 || numberOnLight < 0)
            {
                error = "Stored device record is invalid";
                return false;
            }
            records.push_back(Light(
                id, controllerId, value["name"].as<const char *>(),
                value["room"].as<const char *>(), outputNumber, protocolId,
                roomId, numberOnLight, value["state"].as<bool>()));
        }
        LightStorage::restoreAll(records);
        return true;
    }

    static bool loadControllers(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("controllers", document, found, error) || !found)
            return error.isEmpty();
        std::vector<Controller> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            if (!value["id"].is<int>() || !value["roomId"].is<int>() ||
                value["id"].as<int>() < 1 || value["roomId"].as<int>() < 1)
            {
                error = "Stored controller record is invalid";
                return false;
            }
            records.push_back(Controller(value["id"].as<int>(), value["roomId"].as<int>()));
        }
        ControllerStorage::restoreAll(records);
        return true;
    }

    static bool loadProtocols(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("protocols", document, found, error) || !found)
            return error.isEmpty();
        std::vector<Protocol> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            if (!value["id"].is<int>() || !value["name"].is<const char *>() ||
                !value["kind"].is<int>())
            {
                error = "Stored protocol record is invalid";
                return false;
            }
            int kind = value["kind"].as<int>();
            if (value["id"].as<int>() < 1 || kind < 0 || kind > 1)
            {
                error = "Stored protocol record is invalid";
                return false;
            }
            records.push_back(Protocol(
                value["id"].as<int>(), value["name"].as<const char *>(),
                static_cast<Protocol::Kind>(kind)));
        }
        ProtocolStorage::restoreAll(records);
        return true;
    }

    static bool loadWireless(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("wireless", document, found, error) || !found)
            return error.isEmpty();
        std::vector<WirelessDevice> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            JsonArrayConst mac = value["mac"].as<JsonArrayConst>();
            if (!value["id"].is<int>() || !value["hasMac"].is<bool>() ||
                !value["channel"].is<int>() || !value["protocolId"].is<int>() ||
                mac.size() != 6)
            {
                error = "Stored wireless device record is invalid";
                return false;
            }
            WirelessDevice device = {};
            device.id = value["id"].as<int>();
            device.hasMac = value["hasMac"].as<bool>();
            device.channel = value["channel"].as<int>();
            device.protocolId = value["protocolId"].as<int>();
            if (device.id < 1 || device.protocolId < 1)
            {
                error = "Stored wireless device record is invalid";
                return false;
            }
            for (size_t i = 0; i < 6; ++i)
            {
                if (!mac[i].is<uint8_t>())
                {
                    error = "Stored wireless MAC address is invalid";
                    return false;
                }
                device.mac[i] = mac[i].as<uint8_t>();
            }
            records.push_back(device);
        }
        WirelessStorage::restoreAll(records);
        return true;
    }

    static bool loadSources(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("sources", document, found, error) || !found)
            return error.isEmpty();
        std::vector<ControllerSource> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            if (!value["id"].is<int>() || !value["pinNumber"].is<int>() ||
                !value["controllerId"].is<int>() || !value["buttonType"].is<int>())
            {
                error = "Stored control source record is invalid";
                return false;
            }
            ControllerSource source;
            source.setId(value["id"].as<int>());
            source.setPinNumber(value["pinNumber"].as<int>());
            source.setControllerId(value["controllerId"].as<int>());
            source.setButtonType(value["buttonType"].as<int>());
            if (source.getId() < 1 || source.getPinNumber() < 0 ||
                source.getControllerId() < -1 || source.getButtonType() < -1 ||
                source.getButtonType() > 1)
            {
                error = "Stored control source record is invalid";
                return false;
            }
            records.push_back(source);
        }
        ControllerSourceStorage::restoreSources(records);
        return true;
    }

    static bool loadButtons(String &error)
    {
        JsonDocument document;
        bool found;
        if (!readDocument("buttons", document, found, error) || !found)
            return error.isEmpty();
        std::vector<ButtonSource> records;
        for (JsonObjectConst value : document.as<JsonArrayConst>())
        {
            if (!value["id"].is<int>() || !value["type"].is<int>() ||
                !value["controllerSourceId"].is<int>())
            {
                error = "Stored button record is invalid";
                return false;
            }
            const int id = value["id"].as<int>();
            const int type = value["type"].as<int>();
            const int sourceId = value["controllerSourceId"].as<int>();
            if (id < 1 || type < 0 || type > 1 || sourceId < 1)
            {
                error = "Stored button record is invalid";
                return false;
            }
            ButtonSource button;
            button.setId(id);
            button.setType(static_cast<ButtonSource::Type>(type));
            button.setControllerSourceId(sourceId);
            records.push_back(button);
        }
        ControllerSourceStorage::restoreButtons(records);
        return true;
    }

    static bool accumulate(bool result, const String &categoryError, String &errors)
    {
        if (!result)
        {
            if (!errors.isEmpty())
                errors += "; ";
            errors += categoryError;
        }
        return result;
    }

public:
    static bool saveCategory(const String &category, String &error)
    {
        if (category == "Rooms") return saveRooms(error);
        if (category == "Devices") return saveDevices(error);
        if (category == "Controllers") return saveControllers(error);
        if (category == "Protocols") return saveProtocols(error);
        if (category == "Wireless") return saveWireless(error);
        if (category == "Control Sources") return saveSources(error);
        if (category == "Buttons") return saveButtons(error);
        error = "Unknown data category";
        return false;
    }

    static bool saveAll(String &error)
    {
        bool success = true;
        String categoryError;
        bool result = saveRooms(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = saveDevices(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = saveControllers(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = saveProtocols(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = saveWireless(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = saveSources(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = saveButtons(categoryError);
        success = accumulate(result, categoryError, error) && success;
        return success;
    }

    static bool loadAll(String &error)
    {
        bool success = true;
        String categoryError;
        bool result = loadRooms(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = loadProtocols(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = loadControllers(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = loadSources(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = loadButtons(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = loadDevices(categoryError);
        success = accumulate(result, categoryError, error) && success;
        categoryError = "";
        result = loadWireless(categoryError);
        success = accumulate(result, categoryError, error) && success;
        return success;
    }
};
