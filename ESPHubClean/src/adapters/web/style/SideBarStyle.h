#pragma once
#include <WebServer.h>
class SideBarStyle
{
    static void sendStyle(WebServer * server)
    {

        server->sendContent(R"rawliteral(
<style>
  .sidebar {
    width: 250px;
    position: fixed;
    top: 0;
    left: 0;
    bottom: 0;
    padding: 20px;
    border-right: 1px solid rgba(255, 255, 255, 0.1);

    background-color: rgba(17, 24, 39, 0.95);

    overflow-y: auto;

    transform: translate3d(0,0,0);
    will-change: transform, background-color;
  }
</style>
)rawliteral");
    }
};