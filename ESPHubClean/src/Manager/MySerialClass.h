#pragma once
#include <Arduino.h>
class MySerialClass
{
public:
    MySerialClass()
    {
        Serial.begin(115200);
        Serial.println("Serial was begun");
    }
};