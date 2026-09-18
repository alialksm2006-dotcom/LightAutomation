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
       const dialog = document.getElementById('addControlSourceModal');
       dialog.showModal();
       return ;
      }
    }

    document.getElementById('addControlSourceModal').addEventListener('close', function() {
    document.getElementById('controlSourceForm').reset();});
    )rawliteral");

    
}
};