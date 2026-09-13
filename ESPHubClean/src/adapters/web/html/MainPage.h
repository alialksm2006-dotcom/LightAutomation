#pragma once

#include <ArduinoJson.h>
#include "LightsPage.h"
#include "adapters/web/style/MainStyle.h"
#include "adapters/web/html/SideBarhtml.h"
#include "adapters/web/script/MainScript.h"
#include "adapters/web/script/HandleAddButtonScript.h"
#include "ControlSourcesPage.h"

class MainPage
{
private:
public:
  MainPage()
  {
  }

  static void showMainPage()
{
    EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    EspServer::server.send(200, "text/html", "");

    EspServer::server.sendContent("<!doctype html>");
    EspServer::server.sendContent("<html lang=\"en\">");
    EspServer::server.sendContent("  <head>");
    EspServer::server.sendContent("    <meta charset=\"UTF-8\" />");
    EspServer::server.sendContent("    <title>Smart Home Dashboard</title>");
    EspServer::server.sendContent("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />");



    EspServer::server.sendContent("    <style>");
    MainStyle::sendAllStyles();
    EspServer::server.sendContent("    </style>");


    EspServer::server.sendContent(" </head>");




    EspServer::server.sendContent("<body>");
    EspServer::server.sendContent("<div class=\"main\">");
    EspServer::server.sendContent("  <div class=\"topbar\">");
    EspServer::server.sendContent("    <span class=\"status\">● Connected</span>");
    EspServer::server.sendContent("    <button class=\"btn\" id=\"addButton\" onclick=\"handleAdd()\">Add</button>");
    EspServer::server.sendContent("  </div>");
    EspServer::server.sendContent("  <div id=\"content\">");
    LightsPage::sendTable();
    EspServer::server.sendContent("  </div>");
    EspServer::server.sendContent("</div>");
    LightsPage::sendAddLightModal();
    EspServer::server.sendContent("<div class=\"toast\" id=\"toast\"></div>");
    SideBarhtml::sendHtml();
    ControlSourcesPage::showAdd();
    EspServer::server.sendContent("<script>");
    MainScript::sendMainScript();
    HandleAddButtonScript::send();
    EspServer::server.sendContent("</script>");
    EspServer::server.sendContent("</body>");
    EspServer::server.sendContent("</html>");
}
};
