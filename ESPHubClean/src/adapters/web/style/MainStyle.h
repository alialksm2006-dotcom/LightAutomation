#pragma once
#include <WebServer.h>
#include "adapters/web/style/BadgeStyle.h"
#include "adapters/web/style/ButtonStyle.h"
#include "adapters/web/style/CardStyle.h"
#include "adapters/web/style/FormStyle.h"
#include "adapters/web/style/GlobalStyle.h"
#include "adapters/web/style/GridStyle.h"
#include "adapters/web/style/HtmlBodyStyle.h"
#include "adapters/web/style/ModalStyle.h"
#include "adapters/web/style/SideBarStyle.h"
#include "adapters/web/style/SwitchStyle.h"
#include "adapters/web/style/TableStyle.h"
#include "adapters/web/style/ToastStyle.h"
#include "adapters/web/style/TopBarStyle.h"










class MainStyle
{
  public:
    static void sendStyle(WebServer* server)
    {
        server->sendContent(R"rawliteral(
  .main {
    flex: 1;
    padding: 20px;
    margin-left: 250px;
  }
)rawliteral");
    }

    static void sendAllStyles(WebServer* server)
    {
BadgeStyle::sendStyle(server);
ButtonStyle::sendStyle(server);
CardStyle::sendStyle(server);
FormStyle::sendStyle(server);
GlobalStyle::sendStyle(server);
GridStyle::sendStyle(server);
HtmlBodyStyle::sendStyle(server);
sendStyle(server);
ModalStyle::sendStyle(server);
SideBarStyle::sendStyle(server);
SwitchStyle::sendStyle(server);
TableStyle::sendStyle(server);
ToastStyle::sendStyle(server);
TopBarStyle::sendStyle(server);

    }
};