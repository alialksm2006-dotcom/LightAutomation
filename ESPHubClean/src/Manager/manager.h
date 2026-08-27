#pragma once
#include "adapters/storage/LightsRepo.h"
#include "adapters/MyRemoteXY.h"
#include "infrastructure/EspNowConnection.h"
#include "domain/entities/Light.h"
#include "infrastructure/EspNowConnectionData.h"
#include "adapters/storage/LightsRepo.h"
#include "adapters/gpio_input/RockerSwitch.h"
#include "infrastructure/GpioDriver.h"
#include "usecases/ToggleLightUseCase.h"
#include "infrastructure/GpioConnectionData.h"
#include "infrastructure/GpioConnection.h"
#include "usecases/TurnOffLightUseCase.h"
#include "usecases/TurnOnLightUseCase.h"
// #include "adapters/UiUpdater/RxAdapter.h"
#include "ports/IDashboardPort.h"
#include "adapters/UiUpdater/BlynkAdapter.h"
#include "adapters/BlynkConfig.h"
#include "adapters/web/WebServerAdapter.h"
#include "adapters/storage/SwitchesRepo.h"
#include <vector>

class Manager
{
private:
    std::vector<IDashboardPort *> dashboards;
    SwitchesRepo switchesRepo;

    LightsRepo repo;
    EspNowConnection espNowConnection;
    GpioConnection gpioConnection;
    uint8_t mac1[6] = {0xBC, 0xDD, 0xC2, 0x53, 0x50, 0xBB};

    GpioConnectionData gpioConnectionDataE0;
    GpioConnectionData gpioConnectionDataE1;
    EspNowConnectionData espNowConnectionDataE2;
    EspNowConnectionData espNowConnectionDataE3;

    GpioConnectionData gpioConnectionDataB0;
    GpioConnectionData gpioConnectionDataB1;
    EspNowConnectionData espNowConnectionDataB2;
    EspNowConnectionData espNowConnectionDataB2;


    GpioDriver gpioDriver;

    ToggleLightUseCase toggleUseCase;
    TurnOnLightUseCase turnOn;
    TurnOffLightUseCase turnOff;
    // MyRemoteXY remoteXY;

    // RxAdapter rxAdapter;
    BlynkAdapter blynkAdapter;

    WebServerAdapter webServer;

public:
    Manager() : toggleUseCase(&repo, dashboards),
                turnOn(&repo, dashboards),
                turnOff(&repo, dashboards),
                // remoteXY(turnOn, turnOff),
                // rxAdapter(&remoteXY),
                webServer(&repo, &switchesRepo),
                blynkAdapter(&turnOn, &turnOff)

    {
        // dashboards.push_back(&rxAdapter);
        dashboards.push_back(&blynkAdapter);
    }
    void setup()
    {
        Serial.println("here in setup manager");
        LightsRepo::init();
        // remoteXY.setup();
        Serial.println("here in setup manager after LightsRepo::init() ");
        blynkAdapter.begin(BLYNK_AUTH_TOKEN, BLYNK_SSID, BLYNK_PASS);
        Serial.println("Blynk started");
        switchesRepo.init();
        gpioConnectionDataE0.init(2);
        espNowConnectionDataE3.begin(mac1, 0);

        gpioConnectionDataB0.init(18);
        espNowConnectionDataB2.begin(mac1, 2);
       

        IRepoPort::SmartDevice deviceE0 = {Light(0, 0, "E0 "), &gpioConnectionDataE0, &gpioConnection};

        repo.addDevice(deviceE0);

        espNowConnection.begin();
        for (IRepoPort::SmartDevice &device : repo.getDevices())
        {
            device.executeConnection->init(device.lightConnectionData);
        }

        webServer.begin();
    }

    void update()
    {
        // remoteXY.loop();
        blynkAdapter.run();
        webServer.handle();
        switchesRepo.update(toggleUseCase);
    }
};