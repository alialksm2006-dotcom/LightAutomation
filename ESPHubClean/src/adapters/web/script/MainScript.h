#pragma once
#include <WebServer.h>
class MainScript
{
public :
static void sendMainScript()
{
     EspServer::server.sendContent(R"rawliteral(
         

        let selectedItem = "Devices";
    function showItem(url)
    {
    switch (url) {
        case "/showControlSources":
            selectedItem = "Control Sources";
            break;

        case "/showDevices":
            selectedItem = "Devices";
            break;

        case "/showRooms":
            selectedItem = "Rooms";
            break;

        case "/showControllers":
            selectedItem = "Controllers";
            break;

        case "/showProtocols":
            selectedItem = "Protocols";
            break;

        default:
            selectedItem = "";
    }
    fetch(url)
    .then(response=>response.text())
    .then(html=>showContent(html))
        }

      function showContent(html) {
   
        document.getElementById("content").innerHTML = html;
    }
  
        )rawliteral");
}
};