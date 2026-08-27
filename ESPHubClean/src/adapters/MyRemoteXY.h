#pragma once
#include "RemoteXYGenerated.h"
#include "usecases/TurnOnLightUseCase.h"
#include "usecases/TurnOffLightUseCase.h"
class MyRemoteXY
{
private:
  TurnOnLightUseCase turnOnLightUseCase;
  TurnOffLightUseCase turnOffLightUseCase;

public:
  MyRemoteXY(TurnOnLightUseCase turnOnLightUseCase, TurnOffLightUseCase turnOffLightUseCase)
      : turnOnLightUseCase(turnOnLightUseCase), turnOffLightUseCase(turnOffLightUseCase) {}

  struct CheckChange
  {
    int lightId;
    uint8_t lastState = 0;
    uint8_t currentState = 0;
  };
  CheckChange buttons[4];

  void setup()
  {
    inSetup();
while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.println("connect.. in void setup");
    }
    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
    {
      buttons[i].lastState = 0;
      buttons[i].currentState = 0;
    }
    buttons[0].lightId = 0;
    buttons[1].lightId = 4;
    buttons[2].lightId = 7;
    buttons[3].lightId = 3;
  }
  void loop()
  {
    inLoop();
    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
    {
      switch (i)
      {
      case 0:
        buttons[i].currentState = RemoteXY.pushSwitch_01;
        break;
      case 1:
        buttons[i].currentState = RemoteXY.pushSwitch_02;
        break;
      case 2:
        buttons[i].currentState = RemoteXY.pushSwitch_03;
        break;
      case 3:
        buttons[i].currentState = RemoteXY.pushSwitch_04;
        break;
      }
      if (buttons[i].currentState != buttons[i].lastState)
      {
        buttons[i].lastState = buttons[i].currentState;
        // TelnetStream.println("Change Detected");
        if (buttons[i].currentState)
        {
          turnOnLightUseCase.execute(buttons[i].lightId,"RemoteXY");
        }
        else
        {
          turnOffLightUseCase.execute(buttons[i].lightId,"RemoteXY");
        }
      }
    }
  }

  void updateDashboard(int lightId, int newState)
  {
    if (lightId == buttons[0].lightId)
    {
      RemoteXY.pushSwitch_01 = newState ? 1 : 0;
      buttons[0].lastState = newState;
    }
    else if (lightId == buttons[1].lightId)
    {
      RemoteXY.pushSwitch_02 = newState ? 1 : 0;
      buttons[1].lastState = newState;
    }

    else if (lightId == buttons[2].lightId)
    {
      RemoteXY.pushSwitch_03 = newState ? 1 : 0;
      buttons[2].lastState = newState;
    }
    else if (lightId == buttons[3].lightId)
    {
      RemoteXY.pushSwitch_04 = newState ? 1 : 0;
      buttons[3].lastState = newState;
    }
  }

};