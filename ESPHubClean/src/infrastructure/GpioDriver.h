#include "ports/IGpioDriver.h"
#include <Arduino.h>
class GpioDriver : public IGpioDriver {
public:
    bool read(int pin) override {
        return digitalRead(pin); 
    }
};
