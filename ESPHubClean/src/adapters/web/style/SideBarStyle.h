#pragma once
#include <WebServer.h>
class SideBarStyle
{
  public:
    static void sendStyle()
    {

         EspServer::server.sendContent(R"rawliteral(
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
    .sidebar h2 {
        margin-bottom: 30px;
        color: #38bdf8;
      }

      .sidebar ul {
        list-style: none;
      }

      .sidebar li {
        padding: 12px;
        margin-bottom: 10px;
        border-radius: 10px;
        cursor: pointer;
        transition: 0.5s;
      }

      .sidebar li:hover {
        background: rgba(255, 255, 255, 0.1);
        transform: scale(1.05);
      }

)rawliteral");
    }
};