
#pragma once
#include <Arduino.h>
#include "usecases/ToggleLightUseCase.h"
#include "ports/IGpioDriver.h"
#include <Bounce2.h>
#include "ports/ISwitch.h"

class RockerSwitch : public ISwitch
{
private:
    struct Button
    {
        int pin;
        bool lastState;
        int deviceId;
    };

    Bounce bounce;
    Button button;
    int deviceId;

public:

RockerSwitch(int pinNum, int devId) 
    {
        button.pin = pinNum;
        button.deviceId = devId;
        button.lastState = HIGH; 
    }
    void setup()override
    {
        bounce.attach(button.pin, INPUT_PULLUP);
        bounce.interval(25);    
    }

    void update(ToggleLightUseCase toggle)override
    {
        bounce.update();
        bool current = bounce.read();

        if (current != button.lastState)
        {
            button.lastState = current;

            toggle.execute(button.deviceId, "RockerSwitch");

        }
    }
    std::string getType() override
    {
        return "RockerSwitch /Main Controller + pin : " + std::to_string(button.pin);
    }

    int getPin()
    {
        return button.pin;
    }
    int getDeviceId()
    {
        return button.deviceId;
    }
};