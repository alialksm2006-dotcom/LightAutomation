#pragma once
#include <WebServer.h>
#include <ports/IRepoPort.h>
#include <ports/IRepoSwitch.h>
#include <ArduinoJson.h>
#include "ControlSourcesPage.h"
#include "LightsPage.h"
#include "adapters/web/style/MainStyle.h"

class MainPage
{
private:
  WebServer server;

public:
  MainPage() : server(80)
  {
  }

  void showMainPage()
  {
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");

    server.sendContent(R"rawliteral(<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <title>Smart Home Dashboard</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <style>
    )rawliteral");
MainStyle::sendAllStyles(&server);
    server.sendContent(R"rawliteral(
    </style>
  </head>

  <body>
   
    
    <div class="main">
  
      <div class="topbar">
          <span class="status">● Connected</span>
      </div>

      <div id="content">)rawliteral");

    LightsPage::sendTable(&server);

    server.sendContent(R"rawliteral(
    </div>
    </div>
    )rawliteral");



        LightsPage::sendAddLightModal(&server);
   server.sendContent("<div class=\"toast\" id=\"toast\"></div>");


    server.sendContent("<script>");

    



     
  
      
    server.sendContent("</script>");
  server.sendContent("</body>");
  server.sendContent("</html>");

  }


  void begin()
  {

    WiFi.begin("Ali", "111111111");
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Connecting");
      delay(500);
    }
    Serial.println(WiFi.localIP());

   


    server.begin();
  }

  void handle()
  {
    server.handleClient();
  }
};
