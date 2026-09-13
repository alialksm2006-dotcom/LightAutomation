#pragma once
#include "adapters/api/EspServer.h"
class HandleAddButtonScript
{public:
static void send()
{
EspServer::server.sendContent(R"rawliteral(
    function handleAdd()
    {
    if (selectedItem=="Control Sources")
      {
       fetch("/ControlSources/showAdd");
       const dialog = document.getElementById('addControlSourceModal');
       dialog.showModal();
       return ;
      }
    }
    )rawliteral");
}
};