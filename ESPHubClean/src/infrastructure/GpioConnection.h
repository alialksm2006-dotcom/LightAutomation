#pragma once 
#include "ports/ILightExecuteConnection.h"
#include "domain/entities/Light.h"
#include "GpioConnectionData.h"
#include <Arduino.h>
class GpioInfra :public ILightExecuteConnection
{

    public :


    
void execute(const Light& light, const ILightConnectionData* infra)     {
      const GpioConnectionData *gpio = dynamic_cast<const GpioConnectionData *>(infra);
         if(!gpio)return ;
     digitalWrite(gpio->getPin(),!light.state);
    }

    void init(const ILightConnectionData *infra)override 
    {
 const GpioConnectionData *gpio = dynamic_cast<const GpioConnectionData *>(infra);

   if(!gpio)return ;


    pinMode(gpio->getPin(),OUTPUT);

    }
};