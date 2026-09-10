#pragma once

#include <WebServer.h>
#include "adapters/data_manager/ControllerSourceStorage.h"

class ControlSourcesPage
{
public:

  static void show(WebServer *server)
{
    server->setContentLength(CONTENT_LENGTH_UNKNOWN);
    server->sendContent(R"rawliteral(
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
        server->sendContent("<tr>");

        server->sendContent("<td>");
        server->sendContent(String(source->getId()));
        server->sendContent("</td>");

        server->sendContent("<td>");
        server->sendContent(String(source->getPinNumber()));
        server->sendContent("</td>");

        server->sendContent("<td>");
        server->sendContent(String(source->getControllerId()));
        server->sendContent("</td>");

        server->sendContent("<td>");

        std::string details = source->getDetails();
        server->sendContent(details.c_str());

        server->sendContent("</td>");

        server->sendContent(R"rawliteral(<td> 
              <button class='btn'>-Delete</button> 
               </td>
        )rawliteral");

        server->sendContent("</tr>");
    }

    server->sendContent(R"rawliteral(
            </tbody>
        </table>
    )rawliteral");
}

};
