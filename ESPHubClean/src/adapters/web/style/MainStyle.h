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
#include "adapters/api/EspServer.h"
#include "DialogModalStyle.h"








class MainStyle
{
  public:
    static void sendStyle()
    {
         EspServer::server.sendContent(R"rawliteral(
  .main {
    flex: 1;
    padding: 20px;
    margin-left: 250px;
  }
)rawliteral");
    }

    static void sendAllStyles()
    {
       
BadgeStyle::sendStyle();
ButtonStyle::sendStyle();
CardStyle::sendStyle();
FormStyle::sendStyle();
GlobalStyle::sendStyle();
GridStyle::sendStyle();
HtmlBodyStyle::sendStyle();
sendStyle();
ModalStyle::sendStyle( );
SideBarStyle::sendStyle( );
SwitchStyle::sendStyle( );
TableStyle::sendStyle( );
ToastStyle::sendStyle( );
TopBarStyle::sendStyle( );
DialogModalSyle::sendStyle();

    }
};