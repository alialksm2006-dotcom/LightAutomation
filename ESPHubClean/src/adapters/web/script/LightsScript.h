#pragma once
#include <WebServer.h>
class LightsScript
{
public:
    static void sendAddLightScript(WebServer *server)
    {
        server->sendContent(R"rawliteral(
    function openModal() {
        document.getElementById("modal").style.display = "flex";
        onProtocolChange();
      }

      function closeModal() {
        document.getElementById("modal").style.display = "none";
        clearForm();
      }

      function clearForm() {
        document.getElementById("name").value = "";
        document.getElementById("room").value = "";
        document.getElementById("controllerId").value = "";
        document.getElementById("protocol").value = "GPIO";
        document.getElementById("mac").value = "";
        document.getElementById("pin").value = "";
        onProtocolChange();
      }

      function onProtocolChange() {
        const protocol = document.getElementById("protocol").value;
        const macGroup = document.getElementById("macGroup");
        const macInput = document.getElementById("mac");

        if (protocol === "ESP-NOW") {
          macGroup.classList.remove("hidden");
          macInput.required = true;
        } else {
          macGroup.classList.add("hidden");
          macInput.required = false;
          macInput.value = "";
        }
      }

      function isValidMac(mac) {
        return /^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$/.test(mac.trim());
      }

      

      function addDeviceEntity() {

    const name = document.getElementById("name").value.trim();
    const room = document.getElementById("room").value.trim();
    const controllerId = document.getElementById("controllerId").value.trim();
    const protocol = document.getElementById("protocol").value;
    const mac = document.getElementById("mac").value.trim();
    const pin = document.getElementById("pin").value.trim();
    const switchPin = document.getElementById("switchPin").value.trim();

    if (!name || !room || !controllerId || !pin || !switchPin) {
        showToast("Please fill all required fields");
        return;
    }

    if (protocol === "ESP-NOW" && !isValidMac(mac)) {
        showToast("Enter a valid MAC like AA:BB:CC:DD:EE:FF");
        return;
    }

    const payload = {
        name,
        room,
        controllerId: parseInt(controllerId),
        protocol,
        mac: protocol === "ESP-NOW" ? mac : "",
        pin: parseInt(pin),
        switchPin: parseInt(switchPin),
        state: false
    };

    fetch("/api/device/add", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(payload)
    })
    .then(async (res) => {

        if (!res.ok) {
            const msg = await res.text();
            throw new Error(msg || "Failed to add device");
        }

        return res.text();
    })
    .then(() => {

        closeModal();
        showToast("Device added");

        return fetch("/api/switch/add", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                switchPin: parseInt(switchPin)
            })
        });
    })
    .then(async (res) => {

        if (!res.ok) {
            const msg = await res.text();
            throw new Error(msg || "Failed to add switch");
        }

        return res.text();
    })
    .then(() => {
        showToast("Device and Switch added successfully");
        location.reload();
    })
    .catch((err) => {
        showToast(err.message);
    });
}


     function addDevice() {

    switch (selectedItem) {

        case "Devices":
            addDeviceEntity();
            break;

        case "Control Sources":
            addControlSourceEntity();
            break;

        case "Controllers":
            addControllerEntiy();
            break;

        case "Rooms":
            addRoomEntity();
            break;

        case "Protocols":
            addProtocolEntity();
            break;

        default:
            showToast("Unknown item selected");
            break;
    }
}



      function deleteDevice(id) {
  if (!confirm("Delete this device?")) return;

  fetch("/api/device/delete", {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({ id: id })
  })
  .then(async (res) => {
    if (!res.ok) {
      const txt = await res.text();
      throw new Error(txt || "Delete failed");
    }
    showToast("Device deleted");
    location.reload();
  })
  .catch((err) => {
    showToast(err.message);
  });
}
    )rawliteral");
    }
};