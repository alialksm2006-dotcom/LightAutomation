#pragma once
#include <string>

class UIItem {
public:
    std::string type;      // switch, slider, button
    std::string label;     // "Lamp Bedroom"
    std::string indicator; // led, none
    std::string deviceId;  // الرابط بالجهاز أو المصباح
};