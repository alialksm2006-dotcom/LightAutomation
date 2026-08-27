#pragma once
#include <Arduino.h>
#include "ports/ILightConnectionData.h"
#include <TelnetStream.h>
#include <string>
class EspNowConnectionData : public ILightConnectionData
{
public:
    uint8_t mac[6];
    int pin;
    void begin(uint8_t macAddress[6], int pinNumber)
    {
        memcpy(mac, macAddress, 6);
        pin = pinNumber;
    }

    std::string getProtocol() override
    {
        return "ESP-NOW : Wireless";
    }

    std::string connectionString() const
    {
        char macStr[18];
        sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return std::string(macStr) + "\nGPIO pin: " + std::to_string(pin);
    }
};