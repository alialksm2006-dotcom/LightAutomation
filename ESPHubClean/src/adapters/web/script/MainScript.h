#pragma once
#include <WebServer.h>
class MainScript
{
public :
static void sendMainScript()
{
     EspServer::server.sendContent(R"rawliteral(
         

let selectedItem = "Devices";
const nativeFetch = window.fetch.bind(window);
window.fetch = async function(...args) {
    const response = await nativeFetch(...args);
    if (response.status === 401 && window.location.pathname !== "/login")
        window.location.assign("/login");
    return response;
};

const entityRoutes = {
    "Devices": "/showDevices",
    "Rooms": "/showRooms",
    "Controllers": "/showControllers",
    "Protocols": "/showProtocols",
    "Wireless": "/showWireless",
    "Control Sources": "/showControlSources",
    "Buttons": "/showButtons"
};

function setActiveNavigation(route) {
    document.querySelectorAll(".sidebar .nav-item").forEach(item => {
        item.classList.toggle("active", item.dataset.navRoute === route);
    });
}

document.addEventListener("click", event => {
    const target = event.target;
    if (!(target instanceof Element)) return;
    const clickable = target.closest(
        "button, .sidebar .nav-item, summary, a, input[type='checkbox'], input[type='radio']"
    );
    if (!clickable) return;
    clickable.classList.remove("is-pressed");
    void clickable.offsetWidth;
    clickable.classList.add("is-pressed");
    window.setTimeout(() => clickable.classList.remove("is-pressed"), 240);

    if (clickable.matches(".sidebar .nav-item"))
        setActiveNavigation(clickable.dataset.navRoute);
}, true);

async function showItem(url) {
    const routeItem = Object.keys(entityRoutes).find(name => entityRoutes[name] === url);
    const previousItem = selectedItem;
    if (routeItem) {
        selectedItem = routeItem;
        setActiveNavigation(url);
    }
    try {
        const response = await fetch(url);
        if (!response.ok) throw new Error("Could not load " + selectedItem);
        document.getElementById("content").innerHTML = await response.text();
    } catch (error) {
        if (routeItem) {
            selectedItem = previousItem;
            setActiveNavigation(entityRoutes[previousItem]);
        }
        showToast(error.message, true);
        console.error(error);
    }
}

document.addEventListener("DOMContentLoaded", () => showItem(entityRoutes.Devices));
  
        )rawliteral");
}
};