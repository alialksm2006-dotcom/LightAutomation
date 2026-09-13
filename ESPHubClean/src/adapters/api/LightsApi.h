#pragma once
#include <adapters/api/EspServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "adapters/web/html/LightsPage.h"/

class LightsApi
{
    private:
    
  static void addDevice(WebServer* server)
  {
    String body = server->arg("plain");

    Serial.println("API HIT");
    Serial.println(body);
    DynamicJsonDocument doc(512);

    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
      server->send(400, "text/plain", "Invalid JSON");
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
     
    }
    else
    {
      server->send(400, "text/plain", "Unknown protocol");
      return;
    }

    server->send(200, "text/plain", "OK");
  }

public : 
static void beginAddDeviceApi(WebServer* server)
{
//  server->on("/api/device/add", HTTP_POST, []()
//               { addDevice(server); });

}
static void begin()
{
  EspServer::server.on("/devices/show",HTTP_GET,LightsPage::sendTable);
}
};