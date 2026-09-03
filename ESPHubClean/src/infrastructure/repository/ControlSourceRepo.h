#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <vector>
#include <memory>

#include "adapters/data_manager/ControllerSourceStorage.h"

class ControllerSourceRepo
{
private:
    Preferences preferences;

public:
    void save()
    {
        preferences.begin("ctrl_src", false);

        preferences.clear();
int count = ControllerSourceStorage::getAll().size();
        preferences.putUInt("count", count);

        for (size_t i = 0; i < count; i++)
        {
            String prefix = "src" + String(i) + "_";

            preferences.putInt(
                (prefix + "id").c_str(),
                ControllerSourceStorage::getAll()[i]->getId()
            );

            preferences.putInt(
                (prefix + "pin").c_str(),
                ControllerSourceStorage::getAll()[i]->getPinNumber()
            );

            if (ButtonSource* button =
                    dynamic_cast<ButtonSource*>(ControllerSourceStorage::getAll()[i].get()))
            {
                preferences.putInt(
                    (prefix + "type").c_str(),
                    static_cast<int>(button->getType())
                );
            }
        }

        preferences.end();
    }

    void load()
    {
        preferences.begin("ctrl_src", true);

        ControllerSourceStorage::getAll().clear();

        size_t count = preferences.getUInt("count", 0);

        for (size_t i = 0; i < count; i++)
        {
            String prefix = "src" + String(i) + "_";

            int id = preferences.getInt(
                (prefix + "id").c_str(),
                0
            );

            int pin = preferences.getInt(
                (prefix + "pin").c_str(),
                0
            );

            int type = preferences.getInt(
                (prefix + "type").c_str(),
                0
            );

            auto button = std::unique_ptr<ButtonSource>(new ButtonSource());

            button->setId(id);
            button->setPinNumber(pin);
            button->setType(
                static_cast<ButtonSource::Type>(type)
            );

            ControllerSourceStorage::getAll().push_back(std::move(button));
        }

        preferences.end();
    }
};
