#pragma once
#include <vector>
#include <Arduino.h>
#include "ports/ILightExecuteConnection.h"
#include "MyEspNowLib.h"
#include "adapters/storage/LightsRepo.h"
#include "EspNowConnectionData.h"
#include <TelnetStream.h>

class EspNowConnection : public ILightExecuteConnection
{
private:
    LightsRepo repo;

    static void onDataRecv(const uint8_t *mac, const uint8_t *data, uint8_t len)
    {
    }

    static void onDataSend(const uint8_t *mac, bool success)
    {
        // TelnetStream.println(success ? "Data sent successfully" : "Data send failed");
    }

public:
    enum Type
    {
        Toggle = 0,
        updateChannel = 1
    };
    struct DataToggle
    {
        Type type;
        bool state;
        int bin;
    };
    DataToggle dataToggle;

    static void begin()
    {
        espNowBegin(ESP_NOW_ROLE_BOTH, WiFi.channel());
        espNowOnReceive(onDataRecv);
        espNowOnSend(onDataSend);

        //test 
      
    }

    void init(const ILightConnectionData *infra) override
    {
        const EspNowWay *infraWay = dynamic_cast<const EspNowWay *>(infra);

        if (!infraWay)
            return;
        espNowAddPeer(infraWay->mac);
    }

    void execute(const Light &light, const ILightConnectionData *infra) override
    {
        Serial.println("mac is : " + String(infra->connectionString().c_str()));
        const EspNowWay *infraWay = dynamic_cast<const EspNowWay *>(infra);
        if (!infraWay)
            return;

        dataToggle.type = Toggle;
        dataToggle.state = light.getState();
        dataToggle.bin = infraWay->pin;
        // TelnetStream.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", infraWay->mac[0], infraWay->mac[1], infraWay->mac[2], infraWay->mac[3], infraWay->mac[4], infraWay->mac[5]);
        espNowSend(infraWay->mac, (const void *)&dataToggle, sizeof(dataToggle));
    }
};