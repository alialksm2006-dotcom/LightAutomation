#pragma once
#include <WebServer.h>
#include <WiFi.h>
class Test
{

public:
    static WebServer server;
    struct User
    {
        int id;
        const char *name;
        int age;
        const char *email;
    };

    static User users[3];

    static void handleRoot()
    {
        server.sendContent(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Web Server</title>
</head>

<body>
    <div id="topbar">TOP BAR</div>

    <div id="sidebar">
        <ul>
            <li onclick="showHome()">Home</li>
            <li onclick="showItem('/users')">Users</li>
            <li onclick="showProducts()">Products</li>
            <li onclick="showSettings()">Settings</li>
        </ul>
    </div>

    <div id="content">
        <h1>Home</h1>
        <p>Welcome to my ESP32 web server</p>
    </div>
</body>

<script>
   
    function showHome() {
        showContent(`
            <h1>Home</h1>
            <p>
                Welcome to my ESP32 web server
            </p>
        `);
    }

    
    function showItem(url)
    {
    fetch(url)
    .then(response=>response.text())
    .then(html=>showContent(html))
        }



    



    function showProducts() {
        showContent(`
            <h1>Products</h1>
            <p>Here are the products</p>
        `);
    }

    function editUser(id) {
        alert(id);
    }

    function deleteUser(id) {
        document.getElementById("user-" + id).remove();
    }

    function showContent(html) {
   
        document.getElementById("content").innerHTML = html;
    }

    function showSettings() {
        showContent(`
            <h1>Settings</h1>

            <p>Device settings</p>

            <label>
                Device Name:
            </label>

            <input type="text">

            <br><br>

            <button>
                Save
            </button>
        `);
    }
</script>

</html>
)rawliteral");
    }
    static void handleUsers()
    {
        server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        server.send(200, "text/html", "");

        String html = R"rawliteral(
        <h1>Users</h1>

        <table border="1">
            <tr>
                <th>ID</th>
                <th>Name</th>
                <th>Age</th>
                <th>Email</th>
                <th>Action</th>
            </tr>
    )rawliteral";

        server.sendContent(html);

        for (auto &user : users)
        {
            String row = "<tr id=\"user-" + String(user.id) + "\">";

            row += "<td>" + String(user.id) + "</td>";
            row += "<td>" + String(user.name) + "</td>";
            row += "<td>" + String(user.age) + "</td>";
            row += "<td>" + String(user.email) + "</td>";

            row += "<td>";
            row += "<button onclick=\"editUser(" + String(user.id) + ")\">Edit</button>";
            row += "<button onclick=\"deleteUser(" + String(user.id) + ")\">Delete</button>";
            row += "</td>";

            row += "</tr>";

            server.sendContent(row);
        }

        server.sendContent(R"rawliteral(
        </table>
    )rawliteral");
    }

    static void begin()
    {
        Serial.begin(115200);
        WiFi.begin("Ali", "111111111");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Connecting..");
            delay(500);
        }
        Serial.println(WiFi.localIP());

        server.on("/", handleRoot);
        server.on("/users", handleUsers);
        server.begin();
    }
    static void loop()
    {
        server.handleClient();
    }
};