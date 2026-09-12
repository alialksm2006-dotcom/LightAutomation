#pragma once
#include <WebServer.h>
class Toast
{
public: 
static void sendToastScript(WebServer * server)
{
    server->sendContent(R"rawliteral(
      function showToast(msg) {
        let t = document.getElementById("toast");
        t.innerText = msg;
        t.style.display = "block";
        setTimeout(() => (t.style.display = "none"), 2000);
      }
)rawliteral");
}
};