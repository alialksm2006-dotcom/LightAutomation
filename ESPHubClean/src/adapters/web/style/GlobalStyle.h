#pragma once
#include <WebServer.h>
class GlobalStyle
{
static void sendStyle(WebServer * server) {
    server->sendContent(R"rawliteral(
<style>
  * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: "Segoe UI", sans-serif;
  }

  html, body {
    overscroll-behavior: none;
    -webkit-overflow-scrolling: auto;
}
  body {
    background: linear-gradient(135deg, #0f172a, #020617);
    color: white;
    display: flex;
    margin: 0;
    padding: 0;
  }

</style>
)rawliteral");
}
};