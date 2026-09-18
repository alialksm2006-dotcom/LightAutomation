#pragma once
#include "adapters/api/EspServer.h"
class DialogModalSyle
{
public:
static void sendStyle()
{
EspServer::server.sendContent(R"rawliteral(
    dialog.modal-dialog {
    border: none;
    background: transparent;
    padding: 0;
    margin: auto;
    max-width: 100%;
    outline: none;
  }

  dialog.modal-dialog::backdrop {
    background: rgba(0, 0, 0, 0.72);
    backdrop-filter: blur(4px);
  }

  .mb-14 {
    margin-bottom: 14px;
  }
    )rawliteral");
}
};