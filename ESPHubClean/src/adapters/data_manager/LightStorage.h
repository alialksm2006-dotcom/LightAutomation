#pragma once
#include <vector>
#include "domain/entities/Light.h"
class LightStorage
{   
private:
static std::vector<Light> lights;
public:
    static const std::vector<Light> &getLights()
    {
        return lights;
    }
static bool addLight(const Light &light)
    {
        lights.push_back(light);
        return true;
    }

};