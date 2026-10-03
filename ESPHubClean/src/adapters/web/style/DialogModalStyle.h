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

  dialog.delete-confirm-modal {
    width: min(92vw, 420px);
    max-width: calc(100vw - 32px);
    max-height: calc(100vh - 32px);
    max-height: calc(100dvh - 32px);
    overflow-y: auto;
    box-sizing: border-box;
  }

  dialog.delete-confirm-modal .modal-content {
    width: 100%;
    max-width: 100%;
    box-sizing: border-box;
  }

  .delete-confirm-content {
    text-align: center;
    padding: clamp(20px, 4vh, 28px);
    border-color: rgba(248, 113, 113, .2);
  }

  .delete-confirm-icon {
    width: 48px;
    height: 48px;
    display: grid;
    place-items: center;
    margin: 0 auto 16px;
    border: 1px solid rgba(248, 113, 113, .25);
    border-radius: 15px;
    color: #fca5a5;
    background: rgba(239, 68, 68, .12);
    font-size: 1.5rem;
    font-weight: 700;
  }

  .delete-confirm-content h3 {
    margin: 0 0 10px;
    color: #f1f5f9;
    font-size: 1.2rem;
  }

  .delete-confirm-content p {
    margin: 0;
    color: #94a3b8;
    line-height: 1.6;
  }

  .delete-confirm-content .modal-actions {
    justify-content: center;
    margin-top: 24px;
  }

  .delete-confirm-button {
    padding: 10px 18px;
    border: 1px solid rgba(248, 113, 113, .35);
    border-radius: 12px;
    color: #fff;
    background: linear-gradient(135deg, #ef4444, #b91c1c);
    font: inherit;
    font-weight: 600;
    cursor: pointer;
    transition: transform .18s ease, filter .18s ease, box-shadow .18s ease;
  }

  .delete-confirm-button:hover {
    filter: brightness(1.1);
    box-shadow: 0 6px 18px rgba(239, 68, 68, .22);
  }

  .delete-confirm-button:active,
  .delete-confirm-button.is-pressed {
    transform: scale(.96);
  }

  .mb-14 {
    margin-bottom: 14px;
  }
    )rawliteral");
}
};