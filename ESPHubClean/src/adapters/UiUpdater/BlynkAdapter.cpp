#include "BlynkAdapter.h"
#include "adapters/BlynkConfig.h"

#include <BlynkSimpleEsp32.h>
#include <TelnetStream.h>


BlynkAdapter* g_blynkAdapter = nullptr;

BlynkAdapter::BlynkAdapter(TurnOnLightUseCase* on, TurnOffLightUseCase* off)
    : turnOn(on), turnOff(off)
{
    g_blynkAdapter = this;
}

void BlynkAdapter::begin(const char* auth, const char* ssid, const char* pass)
{
    Blynk.begin(auth, ssid, pass);
}

void BlynkAdapter::run()
{
    Blynk.run();
}

void BlynkAdapter::handleVirtualPin(uint8_t vpin, int value)
{
    Serial.println("pin is : "+String(vpin));
    if (value)
        turnOn->execute(vpin, "BlynkAdapter");
    else
        turnOff->execute(vpin,"BlynkAdapter");
}

void BlynkAdapter::updateLightState(const Light& light)
{
    TelnetStream.println("llllight id is :"+String(light.id));
    switch (light.id)
    {
        case 1: Blynk.virtualWrite(V0, light.getState()); break;
        case 2: Blynk.virtualWrite(V1, light.getState()); break;
        case 3: Blynk.virtualWrite(V2, light.getState()); break;
        case 4: Blynk.virtualWrite(V3, light.getState()); break;
        case 5: Blynk.virtualWrite(V4, light.getState()); break;
        case 6: Blynk.virtualWrite(V5, light.getState()); break;
        case 7: Blynk.virtualWrite(V6, light.getState()); break;
    }

   
}
 std::string BlynkAdapter::getName()
    {
        return "BlynkAdapter";
    }