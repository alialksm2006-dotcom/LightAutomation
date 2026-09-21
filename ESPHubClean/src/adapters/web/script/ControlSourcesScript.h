#pragma once
#include <WebServer.h>
#include "adapters/api/EspServer.h"

class ControlSourcesScript
{
public:
    static void send()
    {
        EspServer::server.sendContent(R"rawliteral(

function showToast(message, isError = false) {
    const toast = document.getElementById("toast");
    toast.innerText = message;
    toast.style.background = isError ? "#ef4444" : "#22c55e";
    toast.style.display = "block";

    setTimeout(() => {
        toast.style.display = "none";
    }, 3000);
}

document.getElementById('controlSourceForm').addEventListener('submit', function(e) {
    e.preventDefault();

    const formData = new FormData(this);
    const params = new URLSearchParams(formData);

    fetch('/ControlSources/add', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: params
    })
    .then(async response => {
        const text = await response.text();
        
        if (response.ok) {
            showToast(text, false); 
            
          
            setTimeout(() => {
                document.getElementById('addControlSourceModal').close();
                
                if (selectedItem === "Control Sources") {
                    showItem('/showControlSources');
                }
            }, 1500);

        } else {
            showToast(text, true);
        }
    })
    .catch(error => {
        showToast("Error sending request", true);
        console.error('Error:', error);
    });
});







async function deleteControlSource(id) {
    try {
        const response = await fetch(`/ControlSources/delete?id=${id}`, {
            method: "DELETE"
        });

        if (!response.ok) {
            throw new Error("HTTP Error: " + response.status);
        }

        const data = await response.json();

        if (data.success) {
            showToast(data.message, false);

            if (selectedItem === "Control Sources") {
                showItem('/showControlSources');
            }
        } else {
            showToast(data.message, true);
        }

    } catch (error) {
        showToast("Error sending delete request", true);
        console.error("Error:", error);
    }
}

function editControlSource(id) {
    
  
    console.log("Edit requested for ID:", id);
    

    showToast("Edit mode for ID: " + id, false);
    
}
    )rawliteral");
    }
};