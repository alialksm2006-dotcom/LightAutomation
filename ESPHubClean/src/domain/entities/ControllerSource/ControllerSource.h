#pragma once

class ControllerSource
{
private:
    int id;
    int pinNumber;
    int controllerId;

public:
    virtual ~ControllerSource() = default;
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

    int getControllerId() const
    {
        return controllerId;
    }
    void setControllerId(int controllerId)
    {
        this->controllerId = controllerId;
    }
};