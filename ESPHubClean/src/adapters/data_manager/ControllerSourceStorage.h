#pragma once
#include <vector>
#include <memory>
#include "domain/entities/ControllerSource/ControllerSource.h"
#include "domain/entities/ControllerSource/ButtonSource.h"
#include "LightControllerSourceStorage.h"
class ControllerSourceStorage
{
private:
   static std::vector<std::unique_ptr<ControllerSource>> sources;
    static int id;

public:
   ControllerSourceStorage()
    {
}


    bool add(ControllerSource *source)
    {

        if (source == nullptr)
        {
            return false;
        }

        if (LightControllerSourceStorage::isControlSourceAssociatedWithLight(source->getId()))
        {
            return false;
        }
        ++id;
        source->setId(id);

        sources.emplace_back(source);
        return true;
    }

    bool remove(int id)
    {
        for (auto it = sources.begin(); it != sources.end(); ++it)
        {
            if ((*it)->getId() == id)
            {
                sources.erase(it);
                return true;
            }
        }

        return false;
    }
   static  std::vector<std::unique_ptr<ControllerSource>> &getAll() 
    {
        return sources;
    }
};
