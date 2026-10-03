#pragma once

#include <ArduinoJson.h>
#include "adapters/web/style/MainStyle.h"
#include "adapters/web/html/SideBarhtml.h"
#include "adapters/web/script/MainScript.h"
#include "adapters/web/script/HandleAddButtonScript.h"
#include "EntitiesPage.h"
#include "adapters/web/script/EntitiesScript.h"

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
    EspServer::server.sendContent("    <div class=\"topbar-actions\">");
    EspServer::server.sendContent("      <button class=\"btn add-action\" id=\"addButton\" onclick=\"handleAdd()\"><span aria-hidden=\"true\">+</span> Add</button>");
    EspServer::server.sendContent("      <div class=\"toolbar-menu\" id=\"toolbarMenu\">");
    EspServer::server.sendContent("        <button class=\"toolbar-menu-toggle\" id=\"toolbarMenuToggle\" type=\"button\" aria-label=\"Open actions menu\" aria-haspopup=\"true\" aria-expanded=\"false\" onclick=\"toggleToolbarMenu()\">&#8942;</button>");
    EspServer::server.sendContent("        <div class=\"toolbar-menu-panel\" id=\"toolbarMenuPanel\" role=\"menu\" hidden>");
    EspServer::server.sendContent("          <button class=\"toolbar-menu-item\" type=\"button\" role=\"menuitem\" onclick=\"runToolbarAction(saveCurrentData)\"><span class=\"menu-icon\" aria-hidden=\"true\">&#8683;</span>Save current view</button>");
    EspServer::server.sendContent("          <button class=\"toolbar-menu-item\" type=\"button\" role=\"menuitem\" onclick=\"runToolbarAction(saveAllData)\"><span class=\"menu-icon\" aria-hidden=\"true\">&#128190;</span>Save all data</button>");
    EspServer::server.sendContent("          <div class=\"toolbar-menu-divider\" role=\"separator\"></div>");
    EspServer::server.sendContent("          <button class=\"toolbar-menu-item logout-action\" type=\"button\" role=\"menuitem\" onclick=\"runToolbarAction(logout)\"><span class=\"menu-icon\" aria-hidden=\"true\">&#10140;</span>Log out</button>");
    EspServer::server.sendContent("        </div>");
    EspServer::server.sendContent("      </div>");
    EspServer::server.sendContent("    </div>");
    EspServer::server.sendContent("  </div>");
    EspServer::server.sendContent("  <div id=\"content\">");
    EspServer::server.sendContent("  </div>");
    EspServer::server.sendContent("</div>");
    EntitiesPage::sendModal();
    SideBarhtml::sendHtml();
    EspServer::server.sendContent("<script>");
    MainScript::sendMainScript();
    EntitiesScript::send();
    HandleAddButtonScript::send();
    EspServer::server.sendContent("</script>");
    EspServer::server.sendContent("</body>");
    EspServer::server.sendContent("</html>");
}
};
