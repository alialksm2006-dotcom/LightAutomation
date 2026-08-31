#pragma once

class ControllerSource
{
private:
    int id;
    int pinNumber;

public:
    int getId() const
    {
        return id;
    }

    void setId(int id)
    {
        this->id = id;
    }

    int getPinNumber() const
    {
        return pinNumber;
    }

    void setPinNumber(int pinNumber)
    {
        this->pinNumber = pinNumber;
    }
};