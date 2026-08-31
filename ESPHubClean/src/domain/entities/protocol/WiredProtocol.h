#pragma once
#include "Protocol.h"
class WiredProtocol : public Protocol
{
public:
  WiredProtocol(int id, std::string name)
        : Protocol(id, name)
    {
    }
};