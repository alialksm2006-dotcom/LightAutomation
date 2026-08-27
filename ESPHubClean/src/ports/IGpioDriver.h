#pragma once

class IGpioDriver {
public:
    virtual bool read(int pin) = 0;
};