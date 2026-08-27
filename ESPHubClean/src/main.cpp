#include <Arduino.h>
#include "Manager/manager.h"
#include <TelnetStream.h>
#include "Manager/MySerialClass.h"
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
MySerialClass serial;
Manager manager;

void setup()
{   

  
  manager.setup();
  MDNS.begin("ESP32");
Serial.println("here in main setup");
 while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("connect..");
  }
  Serial.println(WiFi.status()==WL_CONNECTED );
  Serial.println(WiFi.localIP());
  ArduinoOTA.begin();
  TelnetStream.begin();
  Serial.println("Channel :");
  Serial.println(WiFi.channel());

}

void loop()
{
  manager.update();
  ArduinoOTA.handle();
}
