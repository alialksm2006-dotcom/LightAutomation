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

        // مُنشئ يأخذ std::string بالمرجع الثابت
        Light(int id, int controllerId, const std::string &name ,const std::string &room="ROOM 1 ")  : id(id), name(name),controllerId(controllerId), room(room) {}

        void turnOn()
        {
            state = true;
        }

        void turnOff()
        {
            state = false;
        }

        bool getState ()const
        {
            return state;
        }
    };