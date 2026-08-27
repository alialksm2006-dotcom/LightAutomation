#pragma once
#include <string>

class NetworkConfig {
public:
    std::string ssid;
    std::string password;
    int channel;             // قناة WiFi أو ESP-NOW
};