#pragma once
#include <vector>
#include <memory>
#include "domain/entities/LightControllerSource.h"
class LightControllerSourceStorage
{
private:
    static std::vector<std::unique_ptr<LightControllerSource>> sources;

public:
    static const std::vector<std::unique_ptr<LightControllerSource>> &getSources()
    {
        return sources;
    }
    static bool isControlSourceAssociatedWithLight(int id)
    {
        for (const auto &source : sources)
        {
            if (source->getControllerSourceId() == id)
            {
                return true;
            }
        }

        return false;
    }
};