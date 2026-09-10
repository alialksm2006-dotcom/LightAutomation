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
}

void loop()
{
  manager.update();
}
