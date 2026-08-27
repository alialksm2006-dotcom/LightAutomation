#pragma once
#include <string>
#include <vector>
#include "Light.h"

class SubController {
public:
    std::string id;            // sub_07
    std::string mac;           // عنوان MAC الخاص بالESP
    int channel;               // عدد المخرجات / القنوات
    std::vector<Light> lights; // مصابيح متصلة بالجهاز
};