#pragma once
#include <string>
#include <vector>
#include "Light.h"

class Room {
public:
    std::string name;           // bedroom, kitchen
    std::vector<Light> lights;  // المصابيح في هذه الغرفة
};