#pragma once 
#include "usecases/ToggleLightUseCase.h"
#include "ports/IGpioDriver.h"
class ISwitch 
{
    public :
   
   virtual ~ISwitch() = default;
   virtual void setup()=0;
   virtual void update(ToggleLightUseCase toggle)=0;
   virtual std::string getType()=0;
};
