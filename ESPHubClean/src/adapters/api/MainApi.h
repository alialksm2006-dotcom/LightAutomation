#pragma once 
#include "ShowPagesApi.h"
#include "EntitiesApi.h"
#include "adapters/espnow/WirelessDiscovery.h"
#include "infrastructure/repository/EntitiesRepository.h"
#include "WebAuth.h"
#include <ESPmDNS.h>
class MainApi
{
public:
 static void begin()
{
 String storageError;
 if (!EntitiesRepository::loadAll(storageError))
     Serial.println("Failed to restore saved application data: " + storageError);
 WiFi.begin("Ali", "111111111");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Connecting");
            delay(500);
        }
        Serial.println(WiFi.localIP());

        MDNS.begin("smart");
        Serial.println("Open the dashboard at https://smart.local/");
        if (!WirelessDiscovery::begin())
            Serial.println("Failed to initialize ESP-NOW wireless discovery");
        const char *requestHeaders[] = {"Cookie"};
        EspServer::server.collectHeaders(requestHeaders, 1);
        ShowPagesApi::begin();
        EntitiesApi::begin();
        EspServer::server.begin();

}
static void handle()
{
    EspServer::server.handleClient();
}
};