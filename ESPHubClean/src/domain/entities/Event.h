#pragma once
#include <string>

class Event {
public:
    std::string deviceId;
    std::string action;  // turn_on, turn_off
    int timestamp;       // وقت تنفيذ الحدث
};