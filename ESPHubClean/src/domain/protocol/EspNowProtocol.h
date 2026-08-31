#pragma once
#include <string>
#include "Protocol.h"
class EspNowProtocol : public Protocol
{
private:
    int mac[6];
    int channel;

public:
    EspNowProtocol(int id, std::string name, int mac[6], int channel)
        : Protocol(id, name), channel(channel)
    {
        for (int i = 0; i < 6; i++)
        {
            this->mac[i] = mac[i];
        }
    }

    int *getMac()
    {
        return mac;
    }

    void setMac(int mac[6])
    {
        for (int i = 0; i < 6; i++)
        {
            this->mac[i] = mac[i];
        }
    }

    int getChannel() const
    {
        return channel;
    }

    void setChannel(int channel)
    {
        this->channel = channel;
    }
};