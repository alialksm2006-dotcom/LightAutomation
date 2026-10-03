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
    static std::vector<ButtonSource> buttons;
    static int id;
    static int buttonId;

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

    static bool update(const ControllerSource &updatedSource)
    {
        for (ControllerSource &source : sources)
        {
            if (source.getId() == updatedSource.getId())
            {
                source = updatedSource;
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

    static bool isControlSource(int id)
    {
        for (const ControllerSource &source : sources)
        {
            if (source.getId() == id && source.getButtonType() < 0)
                return true;
        }
        return false;
    }

    static int addButton(ButtonSource button)
    {
        button.setId(++buttonId);
        buttons.push_back(button);
        return buttonId;
    }

    static bool updateButton(const ButtonSource &updatedButton)
    {
        for (ButtonSource &button : buttons)
        {
            if (button.getId() == updatedButton.getId())
            {
                button = updatedButton;
                return true;
            }
        }
        return false;
    }

    static bool removeButton(int id)
    {
        for (std::vector<ButtonSource>::iterator it = buttons.begin(); it != buttons.end(); ++it)
        {
            if (it->getId() == id)
            {
                buttons.erase(it);
                return true;
            }
        }
        return false;
    }

    static const std::vector<ButtonSource> &getButtons()
    {
        return buttons;
    }

    static void restoreSources(const std::vector<ControllerSource> &records)
    {
        sources = records;
        id = 0;
        for (const ControllerSource &source : sources)
        {
            if (source.getId() > id)
                id = source.getId();
        }
    }

    static void restoreButtons(const std::vector<ButtonSource> &records)
    {
        buttons = records;
        buttonId = 0;
        for (const ButtonSource &button : buttons)
        {
            if (button.getId() > buttonId)
                buttonId = button.getId();
        }
    }

    static bool isControlSourceUsed(int id)
    {
        for (const ButtonSource &button : buttons)
        {
            if (button.getControllerSourceId() == id)
                return true;
        }
        return false;
    }
};
