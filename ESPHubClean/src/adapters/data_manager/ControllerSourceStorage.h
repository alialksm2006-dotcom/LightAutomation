#pragma once
#include <vector>
#include <memory>
#include "domain/entities/ControllerSource/ControllerSource.h"
#include "domain/entities/ControllerSource/ButtonSource.h"
#include "LightControllerSourceStorage.h"
class ControllerSourceStorage
{
private:
    static std::vector<ControllerSource> sources;
    static int id;

public:
    ControllerSourceStorage()
    {
    }

   static bool add(ControllerSource source)
    {

        ++id;
        source.setId(id);

        sources.emplace_back(source);
        return true;
    }

   static bool remove(int id)
    {
        for (auto it = sources.begin(); it != sources.end(); ++it)
        {
            if ((*it).getId() == id)
            {
                sources.erase(it);
                return true;
            }
        }

        return false;
    }
    static std::vector<ControllerSource> &getAll()
    {
        return sources;
    }
  static  bool isEsixt(int id)
    {
        for (const auto &source : sources)
        {
            if (source.getId() == id)
                return true;
        }
        return false;
    }
};
