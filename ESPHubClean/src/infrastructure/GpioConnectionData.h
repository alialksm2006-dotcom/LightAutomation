#pragma once 
#include "ports/ILightConnectionData.h"
#include <string>
class GpioConnectionData :public ILightConnectionData
{
    private:
    int pin ; 
    public:
    void init(int pin) {this->pin = pin;}
    int getPin()const {return pin;}

std::string getProtocol() override
    {
        return "GPIO";
    }
     std::string connectionString()const 
     {
        return "pin"+std::to_string(pin);
     }
};