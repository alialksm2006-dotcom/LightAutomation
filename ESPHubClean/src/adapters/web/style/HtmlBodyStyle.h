#pragma once
#include<WebServer.h>
class HtmlBodyStyle
{public:
    static void sendStyle()
{
     EspServer::server.sendContent(R"rawliteral(
  html, body {
    overscroll-behavior: none; 
    -webkit-overflow-scrolling: auto; 
  }
)rawliteral");
}
};