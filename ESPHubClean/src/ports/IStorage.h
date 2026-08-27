#pragma once 
class IStorage
{
    virtual void save() = 0;
    virtual void load() = 0;
};