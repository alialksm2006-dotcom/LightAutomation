#pragma once

#include "adapters/storage/LightsRepo.h"
#include "adapters/MyRemoteXY.h"
#include "infrastructure/EspNowConnection.h"
#include "domain/entities/Light.h"
// #include "infrastructure/EspNowConnectionData.h"
// #include "adapters/storage/LightsRepo.h"
// #include "adapters/gpio_input/RockerSwitch.h"
#include "infrastructure/GpioDriver.h"
// #include "usecases/ToggleLightUseCase.h"
// #include "infrastructure/GpioConnectionData.h"
#include "infrastructure/GpioConnection.h"
// #include "usecases/TurnOffLightUseCase.h"
// #include "usecases/TurnOnLightUseCase.h"
// #include "adapters/UiUpdater/RxAdapter.h"
#include "ports/IDashboardPort.h"
// #include "adapters/UiUpdater/BlynkAdapter.h"
// #include "adapters/BlynkConfig.h"
// #include "adapters/web/WebServerAdapter.h"
// #include "adapters/storage/SwitchesRepo.h"
#include <vector>
#include "adapters/api/MainApi.h"

class Manager
{
private:
    std::vector<IDashboardPort *> dashboards;
    // SwitchesRepo switchesRepo;
MainPage mainPage;
    // LightsRepo repo;
    EspNowConnection espNowConnection;
    GpioConnection gpioConnection;
    uint8_t mac1[6] = {0xBC, 0xDD, 0xC2, 0x53, 0x50, 0xBB};

    // GpioConnectionData gpioConnectionDataE0;
    // GpioConnectionData gpioConnectionDataE1;
    // EspNowConnectionData espNowConnectionDataE2;
    // EspNowConnectionData espNowConnectionDataE3;

    // GpioConnectionData gpioConnectionDataB0;
    // GpioConnectionData gpioConnectionDataB1;
    // EspNowConnectionData espNowConnectionDataB2;
    // EspNowConnectionData espNowConnectionDataB3;


    GpioDriver gpioDriver;

    // ToggleLightUseCase toggleUseCase;
    // TurnOnLightUseCase turnOn;
    // TurnOffLightUseCase turnOff;
    // MyRemoteXY remoteXY;

    // RxAdapter rxAdapter;
    // BlynkAdapter blynkAdapter;

    // WebServerAdapter webServer;

public:
    Manager() 
    // : toggleUseCase( dashboards),
    //             turnOn(&repo, dashboards),
    //             turnOff(&repo, dashboards),
    //             // remoteXY(turnOn, turnOff),
    //             // rxAdapter(&remoteXY),
    //             blynkAdapter(&turnOn, &turnOff)

    {
        // dashboards.push_back(&rxAdapter);
        // dashboards.push_back(&blynkAdapter);
    }
    void setup()
    {
        // remoteXY.setup();
        // blynkAdapter.begin(BLYNK_AUTH_TOKEN, BLYNK_SSID, BLYNK_PASS);
       
       



        // espNowConnection.begin();
        // for (IRepoPort::SmartDevice &device : repo.getDevices())
        // {
        //     device.executeConnection->init(device.lightConnectionData);
        // }
        MainApi::begin();

    }

    void update()
    {
        MainApi::handle();
        // remoteXY.loop();
        // blynkAdapter.run();
        // switchesRepo.update(toggleUseCase);
    }
};