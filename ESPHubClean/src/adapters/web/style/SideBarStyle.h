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
        transition: background-color .18s ease, color .18s ease, transform .18s ease,
                    box-shadow .18s ease;
      }

      .sidebar li:hover {
        background: rgba(255, 255, 255, 0.1);
        transform: translateX(3px);
      }

      .sidebar .nav-item.active {
        color: #e0f2fe;
        background: linear-gradient(100deg, rgba(14, 165, 233, .24), rgba(99, 102, 241, .16));
        box-shadow: inset 3px 0 #38bdf8;
      }

      .sidebar .nav-item.is-pressed,
      .sidebar .nav-item:active {
        color: #fff;
        background-color: rgba(56, 189, 248, .32);
        transform: scale(.97);
      }

      .sidebar span.nav-item {
        display: block;
        padding: 10px 12px;
        margin: -10px -12px;
        border-radius: 10px;
        cursor: pointer;
        transition: background-color .18s ease, color .18s ease, transform .18s ease;
      }

      .sidebar span.nav-item:hover {
        background: rgba(255, 255, 255, .1);
      }

      .sidebar span.nav-item.active {
        color: #e0f2fe;
        background: linear-gradient(100deg, rgba(14, 165, 233, .24), rgba(99, 102, 241, .16));
        box-shadow: inset 3px 0 #38bdf8;
      }

      .sidebar span.nav-item.is-pressed,
      .sidebar span.nav-item:active {
        transform: scale(.97);
        background: rgba(56, 189, 248, .32);
      }

)rawliteral");
    }
};