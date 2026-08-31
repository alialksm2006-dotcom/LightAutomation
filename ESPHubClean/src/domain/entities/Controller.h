#pragma once

class Controller
{
private:
    int id;
    int roomId;

public:
    Controller(int id, int roomId)
        : id(id), roomId(roomId)
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

    int getRoomId() const
    {
        return roomId;
    }

    void setRoomId(int roomId)
    {
        this->roomId = roomId;
    }
};