#pragma once
#include "EspServer.h"
#include "adapters/web/html/MainPage.h"
#include "adapters/web/html/LoginPage.h"
#include "WebAuth.h"
class ShowPagesApi
{
public:
    static void begin()
    {
        EspServer::server.on("/", HTTP_GET, []()
        {
            if (WebAuth::isAuthenticated())
                MainPage::showMainPage();
            else
                LoginPage::send();
        });
        EspServer::server.on("/login", HTTP_GET, LoginPage::send);
        EspServer::server.on("/api/login", HTTP_POST, WebAuth::login);
        EspServer::server.on("/api/logout", HTTP_POST, []()
        {
            if (!WebAuth::isAuthenticated())
            {
                WebAuth::rejectRequest();
                return;
            }
            WebAuth::logout();
        });
    }
};