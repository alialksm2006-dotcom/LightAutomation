#pragma once 
#include "ShowPagesApi.h"
#include "EntitiesApi.h"
#include "adapters/espnow/WirelessDiscovery.h"
#include <ESPmDNS.h>
class MainApi
{
public:
 static void begin()
{
 WiFi.begin("Ali", "111111111");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Connecting");
            delay(500);
        }
        Serial.println(WiFi.localIP());

        MDNS.begin("smart");
        if (!WirelessDiscovery::begin())
            Serial.println("Failed to initialize ESP-NOW wireless discovery");
        ShowPagesApi::begin();
        EntitiesApi::begin();
        EspServer::server.begin();

}
static void handle()
{
    EspServer::server.handleClient();
}
};