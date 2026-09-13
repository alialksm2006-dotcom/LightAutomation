#pragma once
#include "EspServer.h"
#include "adapters/web/html/MainPage.h"
#include "ControlSourcesApi.h"
class ShowPagesApi
{
public:
    static void begin()
    {
        EspServer::server.on("/", HTTP_GET, MainPage::showMainPage);
    }
};