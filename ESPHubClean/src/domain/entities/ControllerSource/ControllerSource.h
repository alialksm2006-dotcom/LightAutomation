#pragma once

class ControllerSource
{
private:
    int id = 0;
    int pinNumber = 0;
    int controllerId = -1;
    int buttonType = -1;

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

    int getControllerId() const
    {
        return controllerId;
    }
    void setControllerId(int controllerId)
    {
        this->controllerId = controllerId;
    }

    int getButtonType() const
    {
        return buttonType;
    }

    void setButtonType(int buttonType)
    {
        this->buttonType = buttonType;
    }
    
};