#pragma once
#include "EspServer.h"
#include "adapters/web/html/MainPage.h"
class ShowPagesApi
{
public:
    static void begin()
    {
        EspServer::server.on("/", HTTP_GET, MainPage::showMainPage);
        Serial.println("heere in beign()");
    }
};