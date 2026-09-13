#pragma once

#include <WebServer.h>
#include "adapters/data_manager/ControllerSourceStorage.h"

class ControlSourcesPage
{
public:

  static void show()
{
     EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
     EspServer::server.sendContent(R"rawliteral(
        <h3>Control Sources</h3>

        <table border="1">
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Pin Number</th>
                    <th>Controller ID</th>
                    <th>Details</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
    )rawliteral");

    for (const auto& source : ControllerSourceStorage::getAll())
    {
         EspServer::server.sendContent("<tr>");

         EspServer::server.sendContent("<td>");
         EspServer::server.sendContent(String(source->getId()));
         EspServer::server.sendContent("</td>");

         EspServer::server.sendContent("<td>");
         EspServer::server.sendContent(String(source->getPinNumber()));
         EspServer::server.sendContent("</td>");

         EspServer::server.sendContent("<td>");
         EspServer::server.sendContent(String(source->getControllerId()));
         EspServer::server.sendContent("</td>");

         EspServer::server.sendContent("<td>");

        std::string details = source->getDetails();
         EspServer::server.sendContent(details.c_str());

         EspServer::server.sendContent("</td>");

         EspServer::server.sendContent(R"rawliteral(<td> 
              <button class='btn'>-Delete</button> 
               </td>
        )rawliteral");

         EspServer::server.sendContent("</tr>");
    }

     EspServer::server.sendContent(R"rawliteral(
            </tbody>
        </table>
    )rawliteral");
}

static void showAdd()
{
EspServer::server.sendContent(R"rawliteral(
    <dialog  id="addControlSourceModal">
<p>test dialog</p>
    </dialog>
    
    )rawliteral");
}
static void begin()
{
    showAdd();
}

};
