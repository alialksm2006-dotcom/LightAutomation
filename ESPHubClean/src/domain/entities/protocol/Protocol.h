#pragma once

#include <string>

class Protocol
{
public:
    enum class Kind
    {
        WIRED = 0,
        WIRELESS = 1
    };

private:
    int id;
    std::string name;
    Kind kind;

public:
    Protocol(int id, std::string name, Kind kind = Kind::WIRED)
        : id(id), name(name), kind(kind)
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

    Kind getKind() const
    {
        return kind;
    }

    void setKind(Kind kind)
    {
        this->kind = kind;
    }
};