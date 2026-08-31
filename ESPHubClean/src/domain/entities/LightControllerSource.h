#pragma once

class LightControllerSource
{
private:
    int id;
    int lightId;
    int controllerSourceId;

public:
    LightControllerSource(int id, int lightId, int controllerSourceId)
        : id(id), lightId(lightId), controllerSourceId(controllerSourceId)
    {
    }

    int getId() const
    {
        return id;
    }

    void setId(int id)
    {
        this->id = id;
    }

    int getLightId() const
    {
        return lightId;
    }

    void setLightId(int lightId)
    {
        this->lightId = lightId;
    }

    int getControllerSourceId() const
    {
        return controllerSourceId;
    }

    void setControllerSourceId(int controllerSourceId)
    {
        this->controllerSourceId = controllerSourceId;
    }
};