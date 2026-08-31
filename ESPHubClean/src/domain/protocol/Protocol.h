#pragma once

#include <string>

class Protocol
{
private:
    int id;
    std::string name;

public:
    Protocol(int id, std::string name)
        : id(id), name(name)
    {
    }

    int getId() const
    {
        return id;
    }

    void setId(int id)
    {
        this->id = id;
    }

    std::string getName() const
    {
        return name;
    }

    void setName(std::string name)
    {
        this->name = name;
    }
};