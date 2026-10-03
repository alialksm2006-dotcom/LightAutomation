#pragma once
#include <vector>
#include "domain/entities/Light.h"
class LightStorage
{   
private:
static std::vector<Light> lights;
static int nextId;
public:
    static const std::vector<Light> &getLights()
    {
        return lights;
    }

    static void restoreAll(const std::vector<Light> &records)
    {
        lights = records;
        nextId = 0;
        for (const Light &light : lights)
        {
            if (light.id > nextId)
                nextId = light.id;
        }
    }

static int addLight(const Light &light)
    {
        Light storedLight = light;
        storedLight.id = ++nextId;
        lights.push_back(storedLight);
        return storedLight.id;
    }

    static bool updateLight(const Light &light)
    {
        for (Light &storedLight : lights)
        {
            if (storedLight.id == light.id)
            {
                storedLight = light;
                return true;
            }
        }
        return false;
    }

    static bool removeLight(int id)
    {
        for (std::vector<Light>::iterator it = lights.begin(); it != lights.end(); ++it)
        {
            if (it->id == id)
            {
                lights.erase(it);
                return true;
            }
        }
        return false;
    }

    static void updateRoomName(int roomId, const std::string &name)
    {
        for (Light &light : lights)
        {
            if (light.roomId == roomId)
                light.room = name;
        }
    }

};