#pragma once
#include <Arduino.h>
#include "ports/IDashboardPort.h"
#include "usecases/TurnOnLightUseCase.h"
#include "usecases/TurnOffLightUseCase.h"
#include "domain/entities/Light.h"



class BlynkAdapter : public IDashboardPort
{
private:
    TurnOnLightUseCase* turnOn;
    TurnOffLightUseCase* turnOff;

public:
    BlynkAdapter(TurnOnLightUseCase* on, TurnOffLightUseCase* off);

    void begin(const char* auth, const char* ssid, const char* pass);
    void run();

    void handleVirtualPin(uint8_t vpin, int value);

    void updateLightState(const Light& light) override;

    std::string getName() override;
};

extern BlynkAdapter* g_blynkAdapter;