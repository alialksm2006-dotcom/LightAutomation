#pragma once
#include<WebServer.h>
class HtmlBodyStyle
{public:
    static void sendStyle(WebServer *server)
{
    server->sendContent(R"rawliteral(
  html, body {
    overscroll-behavior: none; 
    -webkit-overflow-scrolling: auto; 
  }
)rawliteral");
}
};