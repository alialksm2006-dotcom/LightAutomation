#pragma once
#include "domain/entities/Light.h"
#include "ILightConnectionData.h"
class ILightExecuteConnection
{
public:
    virtual void execute(const Light& light, const ILightConnectionData* infra) = 0;
    virtual void init(const ILightConnectionData* infra)=0;
}; 