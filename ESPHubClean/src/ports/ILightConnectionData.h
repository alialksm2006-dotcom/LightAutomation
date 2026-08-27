#pragma once
#include <string>
class ILightConnectionData {
    public :
     virtual std::string getProtocol()=0;
     virtual  ~ILightConnectionData()=default;
     virtual std::string connectionString() const = 0;
};