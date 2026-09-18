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
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
    )rawliteral");

        for (const auto &source : ControllerSourceStorage::getAll())
        {
            EspServer::server.sendContent("<tr>");

            EspServer::server.sendContent("<td>");
            EspServer::server.sendContent(String(source.getId()));
            EspServer::server.sendContent("</td>");

            EspServer::server.sendContent("<td>");
            EspServer::server.sendContent(String(source.getPinNumber()));
            EspServer::server.sendContent("</td>");

            EspServer::server.sendContent("<td>");
            EspServer::server.sendContent(String(source.getControllerId()));
            EspServer::server.sendContent("</td>");

            EspServer::server.sendContent("<td>");

           

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
   <dialog id="addControlSourceModal" class="modal-dialog">
  <div class="modal-content">
    <div class="modal-header">
      <h3> Add Control Source</h3>
      <button type="button" class="close-btn" onclick="document.getElementById('addControlSourceModal').close()">&times;</button>
    </div>

    <form method="dialog" id="controlSourceForm">
      <div class="form-group mb-14">
        <label for="pinNumber">PIN Number</label>
        <input type="number" id="pinNumber" name="pinNumber" placeholder="Enter Pin Number" required />
      </div>

      <div class="form-group mb-14">
        <label for="controllerId">Controller ID</label>
        <input type="text" id="controllerId" name="controllerId" placeholder=" Enter Controller ID  " required />
      </div>

      <div class="modal-actions">
        <button type="button" class="btn secondary" onclick="document.getElementById('addControlSourceModal').close()">Cancel</button>
        <button type="submit" class="btn">OK</button>
      </div>
    </form>
  </div>
</dialog>
    )rawliteral");
    }
    static void begin()
    {
        showAdd();
    }
};
