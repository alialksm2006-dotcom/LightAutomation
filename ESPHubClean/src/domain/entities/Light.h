#pragma once
#include <string>
class Light
{
public:
    int id;
    int controllerId;
    bool state = false;
    std::string room;
    std::string name;
    int outputNumber;
    int protocolId;
    int roomId;
    int numberOnLight;

 Light(
        int id,
        int controllerId,
        const std::string& name,
        const std::string& room,
        int outputNumber,
        int protocolId,
        int roomId,
        int numberOnLight,
        bool state = false
    )
        : id(id),
          controllerId(controllerId),
          state(state),
          room(room),
          name(name),
          outputNumber(outputNumber),
          protocolId(protocolId),
          roomId(roomId),
          numberOnLight(numberOnLight)
    {
    }
    void turnOn()
    {
        state = true;
    }

    void turnOff()
    {
        state = false;
    }

    bool getState() const
    {
        return state;
    }

    int getOutputNumber() const
    {
        return outputNumber;
    }
    void setOutputNumber(int number)
    {
        outputNumber = number;
    }
    int getProtocolId() const
    {
        return protocolId;
    }
    void setProtocolId(int id)
    {
        protocolId = id;
    }
    int getRoomId() const
    {
        return roomId;
    }
    void setRoomId(int id)
    {
        roomId = id;
    }


  
};