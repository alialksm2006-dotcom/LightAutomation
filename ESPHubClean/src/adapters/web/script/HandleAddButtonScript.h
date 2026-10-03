#pragma once
#include "adapters/api/EspServer.h"
class HandleAddButtonScript
{public:
static void send()
{
EspServer::server.sendContent(R"rawliteral(
function handleAdd() {
    if (selectedItem === "Wireless") {
        startWirelessDiscovery();
        return;
    }
    if (!entityRoutes[selectedItem]) {
        showToast("Select an item first", true);
        return;
    }
    openEntityModal();
}

async function saveData(scope) {
    try {
        const response = await fetch("/api/storage/save", {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: new URLSearchParams({ scope })
        });
        const message = await response.text();
        if (!response.ok) throw new Error(message || "Could not save data");
        showToast(message);
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}

function saveCurrentData() {
    if (!entityRoutes[selectedItem]) {
        showToast("Select an interface before saving", true);
        return;
    }
    saveData(selectedItem);
}

function saveAllData() {
    saveData("all");
}

function toggleToolbarMenu(forceOpen) {
    const menu = document.getElementById("toolbarMenu");
    const toggle = document.getElementById("toolbarMenuToggle");
    const panel = document.getElementById("toolbarMenuPanel");
    const open = typeof forceOpen === "boolean"
        ? forceOpen
        : panel.hidden;
    panel.hidden = !open;
    menu.classList.toggle("open", open);
    toggle.setAttribute("aria-expanded", String(open));
    toggle.setAttribute("aria-label", open ? "Close actions menu" : "Open actions menu");
}

function runToolbarAction(action) {
    toggleToolbarMenu(false);
    action();
}

document.addEventListener("click", event => {
    const menu = document.getElementById("toolbarMenu");
    if (menu && !menu.contains(event.target))
        toggleToolbarMenu(false);
});

document.addEventListener("keydown", event => {
    if (event.key === "Escape")
        toggleToolbarMenu(false);
});

async function logout() {
    try {
        const response = await fetch("/api/logout", { method: "POST" });
        if (!response.ok) throw new Error("Could not log out");
        window.location.assign("/login");
    } catch (error) {
        showToast(error.message, true);
        console.error(error);
    }
}
    )rawliteral");

    
}
};