#pragma once

#include "adapters/api/EspServer.h"

class EntitiesScript
{
public:
    static void send()
    {
        EspServer::server.sendContent(R"rawliteral(
const entityDefinitions = {
    "Devices": {
        type: "device",
        fields: [
            ["name", "Name", "text"],
            ["roomId", "Room", "relation", "room"],
            ["controllerId", "Controller", "relation", "controller"],
            ["protocolId", "Protocol", "relation", "protocol"],
            ["outputNumber", "Output Number", "number", "0"],
            ["numberOnLight", "Number On Light", "number", "0"],
            ["state", "State", "select", "0"]
        ]
    },
    "Rooms": { type: "room", fields: [["name", "Name", "text"]] },
    "Controllers": { type: "controller", fields: [["roomId", "Room", "relation", "room"]] },
    "Protocols": {
        type: "protocol",
        fields: [["name", "Name", "text"], ["kind", "Type", "select", "0", [["0", "Wired"], ["1", "Wireless"]]]]
    },
    "Wireless": {
        type: "wireless",
        fields: [["protocolId", "Protocol", "relation", "wirelessProtocol"]]
    },
    "Control Sources": {
        type: "source",
        fields: [["pinNumber", "Pin Number", "number", "0"], ["controllerId", "Controller", "relation", "controllerSource"]]
    },
    "Buttons": {
        type: "button",
        fields: [
            ["pinNumber", "Pin Number", "number", "0"],
            ["controllerId", "Controller", "relation", "controllerSource"],
            ["buttonType", "Button Type", "select", "0", [["0", "PUSH"], ["1", "SWITCH"]]]
        ]
    }
};

function showToast(message, isError = false) {
    const toast = document.getElementById("toast");
    toast.textContent = message;
    toast.style.background = isError ? "#ef4444" : "#22c55e";
    toast.style.display = "block";
    setTimeout(() => { toast.style.display = "none"; }, 3000);
}

async function createProtocolFromPicker() {
    const name = document.getElementById("newProtocolName").value.trim();
    if (!name) {
        showToast("Enter a protocol name", true);
        return;
    }
    try {
        if (wirelessFlow) {
            selectedNewProtocolName = name;
            await saveWirelessDevice("");
            return;
        }
        const kind = wirelessFlow || activeRelationType === "wirelessProtocol"
            ? "1" : document.getElementById("newProtocolKind").value;
        const params = new URLSearchParams({ type: "protocol", name, kind });
        const response = await fetch("/api/entities/add", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: params
        });
        const id = await response.text();
        if (!response.ok) throw new Error(id || "Could not create protocol");
        const selection = {
            id,
            label: name + (kind === "1" ? " (Wireless)" : " (Wired)")
        };
        if (wirelessFlow) {
            saveWirelessDevice(id);
            return;
        }
        applyRelationSelection(selection);
        showToast("Protocol created and selected");
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

function startWirelessDiscovery() {
    selectedWirelessPeer = null;
    selectedNewProtocolName = "";
    wirelessFlow = false;
    document.getElementById("wirelessDiscoveryResults").textContent = "No devices discovered yet.";
    document.getElementById("wirelessDiscoveryStatus").textContent =
        "Listening for ESP-NOW discovery broadcasts...";
    document.getElementById("wirelessSpinner").style.display = "block";
    document.getElementById("wirelessDiscoveryModal").showModal();
    pollWirelessDiscovery();
    discoveryTimer = setInterval(pollWirelessDiscovery, 1200);
}

function closeWirelessDiscovery() {
    if (discoveryTimer) clearInterval(discoveryTimer);
    discoveryTimer = null;
    document.getElementById("wirelessDiscoveryModal").close();
}

async function pollWirelessDiscovery() {
    try {
        const response = await fetch("/api/wireless/discovery");
        if (!response.ok) throw new Error("Discovery scan failed");
        const peers = await response.json();
        const container = document.getElementById("wirelessDiscoveryResults");
        container.replaceChildren();
        if (!peers.length) {
            container.textContent = "No devices discovered yet.";
            return;
        }
        document.getElementById("wirelessDiscoveryStatus").textContent =
            "Select the discovered ESP, then continue.";
        const table = document.createElement("table");
        table.innerHTML = "<thead><tr><th>Select</th><th>MAC</th><th>Channel</th></tr></thead>";
        const body = document.createElement("tbody");
        peers.forEach(peer => {
            const row = document.createElement("tr");
            const choiceCell = document.createElement("td");
            const choice = document.createElement("input");
            choice.type = "radio";
            choice.name = "wirelessPeer";
            choice.value = peer.mac;
            choice.addEventListener("change", () => {
                selectedWirelessPeer = { mac: peer.mac, channel: peer.channel };
            });
            choiceCell.appendChild(choice);
            const macCell = document.createElement("td");
            macCell.textContent = peer.mac;
            const channelCell = document.createElement("td");
            channelCell.textContent = peer.channel;
            row.append(choiceCell, macCell, channelCell);
            body.appendChild(row);
        });
        table.appendChild(body);
        container.appendChild(table);
        const selected = Array.from(body.querySelectorAll("input[name='wirelessPeer']"))
            .find(choice => choice.value === (selectedWirelessPeer && selectedWirelessPeer.mac));
        if (selected) selected.checked = true;
    } catch (error) {
        document.getElementById("wirelessDiscoveryStatus").textContent = error.message;
        showToast(error.message, true);
        console.error(error);
    }
}

function continueWirelessSetup() {
    const selected = document.querySelector("input[name='wirelessPeer']:checked");
    if (!selected) {
        showToast("Wait for a device and select its MAC", true);
        return;
    }
    selectedWirelessPeer = {
        mac: selected.value,
        channel: Number(selected.closest("tr").lastElementChild.textContent)
    };
    closeWirelessDiscovery();
    wirelessFlow = true;
    openRelationPicker("protocolId", "wirelessProtocol");
}

async function saveWirelessDevice(protocolId) {
    if (!selectedWirelessPeer) {
        showToast("No discovered wireless device selected", true);
        return;
    }
    try {
        const params = new URLSearchParams({
            mac: selectedWirelessPeer.mac
        });
        if (selectedNewProtocolName)
            params.set("protocolName", selectedNewProtocolName);
        else
            params.set("protocolId", protocolId);
        const response = await fetch("/api/wireless/add", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: params
        });
        const result = await response.text();
        if (!response.ok) throw new Error(result || "Could not add wireless device");
        document.getElementById("relationModal").close();
        wirelessFlow = false;
        selectedWirelessPeer = null;
        selectedNewProtocolName = "";
        showToast("Wireless device added");
        await showItem(entityRoutes.Wireless);
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

async function showWirelessDetails(id) {
    try {
        const response = await fetch("/api/wireless/details?id=" + encodeURIComponent(id));
        const content = await response.text();
        if (!response.ok) throw new Error(content || "Could not load protocol details");
        document.getElementById("wirelessDetailsContent").innerHTML = content;
        document.getElementById("wirelessDetailsModal").showModal();
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

function buildEntityFields(definition) {
    const container = document.getElementById("entityFields");
    container.innerHTML = "";
    definition.fields.forEach(([key, label, inputType, defaultValue, options]) => {
        const group = document.createElement("div");
        group.className = "form-group mb-14";
        const labelElement = document.createElement("label");
        labelElement.htmlFor = "entity-" + key;
        labelElement.textContent = label;
        let input;
        if (inputType === "select") {
            input = document.createElement("select");
            (options || [["0", "OFF"], ["1", "ON"]]).forEach(([value, text]) => {
                const option = document.createElement("option");
                option.value = value;
                option.textContent = text;
                input.appendChild(option);
            });
        } else if (inputType === "relation") {
            input = document.createElement("input");
            input.type = "hidden";
            const display = document.createElement("span");
            display.id = "entity-" + key + "-label";
            display.textContent = "Not selected";
            const choose = document.createElement("button");
            choose.type = "button";
            choose.className = "btn secondary";
            choose.textContent = "Choose";
            choose.onclick = () => openRelationPicker(key, defaultValue);
            group.appendChild(labelElement);
            group.appendChild(input);
            group.appendChild(display);
            group.appendChild(choose);
            input.id = "entity-" + key;
            input.name = key;
            input.dataset.relation = defaultValue;
            container.appendChild(group);
            return;
        } else {
            input = document.createElement("input");
            input.type = inputType;
            if (inputType === "number") input.step = "1";
        }
        input.id = "entity-" + key;
        input.name = key;
        if (defaultValue !== undefined) input.value = defaultValue;
        if (inputType === "text") input.required = true;
        group.appendChild(labelElement);
        group.appendChild(input);
        container.appendChild(group);
    });
}

function openEntityModal(button) {
    const definition = entityDefinitions[selectedItem];
    if (!definition) {
        showToast("This view does not support editing", true);
        return;
    }
    const form = document.getElementById("entityForm");
    form.dataset.type = definition.type;
    delete form.dataset.id;
    buildEntityFields(definition);
    document.getElementById("entityModalTitle").textContent = "Add " + selectedItem;
    if (button) {
        form.dataset.id = button.dataset.id;
        document.getElementById("entityModalTitle").textContent = "Edit " + selectedItem;
        definition.fields.forEach(([key]) => {
            const input = form.elements.namedItem(key);
            if (input && button.dataset[key] !== undefined) input.value = button.dataset[key];
            const display = document.getElementById("entity-" + key + "-label");
            if (display && button.dataset[key + "Label"] !== undefined)
                display.textContent = button.dataset[key + "Label"];
            if (key === "kind" && input) input.value = button.dataset.kind || "0";
        });
    }
    document.getElementById("entityModal").showModal();
}

let activeRelationKey = "";
let activeRelationType = "";
let wirelessFlow = false;
let selectedWirelessPeer = null;
let selectedNewProtocolName = "";
let discoveryTimer = null;

async function openRelationPicker(key, relationType) {
    activeRelationKey = key;
    activeRelationType = relationType;
    document.getElementById("relationModalTitle").textContent =
        "Select " + key.replace(/([A-Z])/g, " $1").toLowerCase();
    document.getElementById("newRoomControls").style.display =
        relationType === "room" ? "block" : "none";
    document.getElementById("newProtocolControls").style.display =
        (relationType === "protocol" || relationType === "wirelessProtocol")
            ? "block" : "none";
    document.getElementById("newRoomName").value = "";
    document.getElementById("newProtocolName").value = "";
    const wirelessProtocolOnly = wirelessFlow || relationType === "wirelessProtocol";
    document.getElementById("newProtocolKind").value = wirelessProtocolOnly ? "1" : "0";
    document.getElementById("newProtocolKind").disabled = wirelessProtocolOnly;
    const options = document.getElementById("relationOptions");
    options.textContent = "Loading...";
    try {
        const response = await fetch("/api/entities/options?type=" + encodeURIComponent(relationType));
        if (!response.ok) throw new Error("Could not load related records");
        options.innerHTML = await response.text();
        document.getElementById("relationModal").showModal();
        const currentValue = document.getElementById("entity-" + key).value;
        if (currentValue) {
            const currentChoice = Array.from(options.querySelectorAll("input[name='relationChoice']"))
                .find(choice => choice.value === currentValue);
            if (currentChoice) currentChoice.checked = true;
        }
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

function confirmRelationSelection() {
    const selected = document.querySelector("input[name='relationChoice']:checked");
    if (!selected) {
        showToast("Select a row first", true);
        return;
    }
    const selection = { id: selected.value, label: selected.dataset.label };
    if (wirelessFlow) {
        saveWirelessDevice(selection.id);
        return;
    }
    applyRelationSelection(selection);
}

function applyRelationSelection(selection) {
    const input = document.getElementById("entity-" + activeRelationKey);
    input.value = selection.id;
    document.getElementById("entity-" + activeRelationKey + "-label").textContent = selection.label;
    closeRelationPicker();
}

function closeRelationPicker() {
    document.getElementById("relationModal").close();
}

async function createRoomFromPicker() {
    const name = document.getElementById("newRoomName").value.trim();
    if (!name) {
        showToast("Enter a room name", true);
        return;
    }
    try {
        const params = new URLSearchParams({ type: "room", name });
        const response = await fetch("/api/entities/add", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: params
        });
        const id = await response.text();
        if (!response.ok) throw new Error(id || "Could not create room");
        applyRelationSelection({ id, label: name });
        showToast("Room created and selected");
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

function closeEntityModal() {
    document.getElementById("entityModal").close();
    document.getElementById("entityForm").reset();
}

function editEntity(button) {
    openEntityModal(button);
}

document.getElementById("entityForm").addEventListener("submit", async function(event) {
    event.preventDefault();
    const missingRelation = Array.from(this.querySelectorAll("input[data-relation]"))
        .find(input => !input.value);
    if (missingRelation) {
        showToast("Choose " + missingRelation.dataset.relation + " before saving", true);
        return;
    }
    const params = new URLSearchParams(new FormData(this));
    params.set("type", this.dataset.type);
    const editing = Boolean(this.dataset.id);
    if (editing) params.set("id", this.dataset.id);
    try {
        const response = await fetch(editing ? "/api/entities/update" : "/api/entities/add", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: params
        });
        const message = await response.text();
        if (!response.ok) throw new Error(message || "Save failed");
        closeEntityModal();
        showToast(editing ? "Updated successfully" : "Added successfully");
        await showItem(entityRoutes[selectedItem]);
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
});

async function deleteEntity(button) {
    if (!confirm("Delete this item?")) return;
    const params = new URLSearchParams({ type: button.dataset.type, id: button.dataset.id });
    try {
        const response = await fetch("/api/entities/delete", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: params
        });
        const message = await response.text();
        if (!response.ok) throw new Error(message || "Delete failed");
        showToast("Deleted successfully");
        await showItem(entityRoutes[selectedItem]);
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}
        )rawliteral");
    }
};
