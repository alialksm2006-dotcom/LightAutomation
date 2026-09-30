#pragma once

#include <Arduino.h>

class WirelessDiscovery
{
private:
    static void onReceive(const uint8_t *mac, const uint8_t *data, uint8_t length);

public:
    static bool begin();
    static bool take(uint8_t mac[6], uint8_t &channel);
};
