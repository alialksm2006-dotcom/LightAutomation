#pragma once


#include <ArduinoJson.h>
#include "LightsPage.h"
#include "adapters/web/style/MainStyle.h"
#include "adapters/web/html/SideBarhtml.h"


class MainPage
{
private:
 

public:
  MainPage() 
  {
  }

 static  void showMainPage()
  {
    EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    EspServer::server.send(200, "text/html", "");

    EspServer::server.sendContent(R"rawliteral(<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <title>Smart Home Dashboard</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <style>
    )rawliteral");
MainStyle::sendAllStyles();
     EspServer::server.sendContent(R"rawliteral(
    </style>
  </head>

  <body>
   
    
    <div class="main">
  
      <div class="topbar">
          <span class="status">● Connected</span>
      </div>

      <div class="btn">
      <button>Add</button>
      </div>
      <div id="content">)rawliteral");

    LightsPage::sendTable();

     EspServer::server.sendContent(R"rawliteral(
    </div>
    </div>
    )rawliteral");



        LightsPage::sendAddLightModal();
    EspServer::server.sendContent("<div class=\"toast\" id=\"toast\"></div>");

    SideBarhtml::sendHtml();

     EspServer::server.sendContent("<script>");

    



     
  
      
     EspServer::server.sendContent("</script>");
   EspServer::server.sendContent("</body>");
   EspServer::server.sendContent("</html>");

  }


  

 
};
