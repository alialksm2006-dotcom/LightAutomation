#pragma once 
#include "ShowPagesApi.h"
#include "ControlSourcesApi.h"
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

        ShowPagesApi::begin();
        ControlSourceApi::begin();
        EspServer::server.begin();

}
static void handle()
{
    EspServer::server.handleClient();
}
};