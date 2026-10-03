#pragma once

#include "adapters/api/EspServer.h"

class LoginPage
{
public:
    static void send()
    {
        EspServer::server.sendHeader("Cache-Control", "no-store");
        EspServer::server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        EspServer::server.send(200, "text/html", "");
        EspServer::server.sendContent(R"rawliteral(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="theme-color" content="#0b1220">
  <title>Sign in | Smart Home</title>
  <style>
    * { box-sizing: border-box; }
    body {
      margin: 0; min-height: 100vh; display: grid; place-items: center;
      padding: 24px; color: #e5edf8;
      font-family: "Segoe UI", Tahoma, Arial, sans-serif;
      background: radial-gradient(ellipse at 15% 10%, #173354 0, transparent 42%),
                  radial-gradient(ellipse at 90% 85%, #28204b 0, transparent 38%),
                  #0b1220;
    }
    .login-card {
      width: min(100%, 440px); padding: 42px;
      border: 1px solid rgba(148, 163, 184, .2); border-radius: 24px;
      background: rgba(17, 27, 44, .88);
      box-shadow: 0 28px 80px rgba(0, 0, 0, .42);
      backdrop-filter: blur(18px);
    }
    .brand {
      display: flex; align-items: center; gap: 14px; margin-bottom: 34px;
    }
    .brand-icon {
      width: 52px; height: 52px; display: grid; place-items: center;
      border-radius: 16px; color: #dff7ff; font-size: 25px;
      background: linear-gradient(145deg, #0ea5e9, #6366f1);
      box-shadow: 0 8px 24px rgba(14, 165, 233, .25);
    }
    .brand-name { font-size: 15px; color: #94a3b8; margin-bottom: 3px; }
    h1 { margin: 0; font-size: 28px; letter-spacing: -.4px; }
    .subtitle { margin: 10px 0 28px; color: #9aa9bd; line-height: 1.8; }
    .field { margin: 0 0 19px; }
    label { display: block; margin-bottom: 9px; font-size: 14px; color: #d6e0ee; }
    input {
      width: 100%; height: 50px; padding: 0 15px; border-radius: 12px;
      border: 1px solid #334155; outline: none; color: #f8fafc;
      background: #0c1524; font: inherit; transition: border-color .2s, box-shadow .2s;
    }
    input:focus { border-color: #38bdf8; box-shadow: 0 0 0 3px rgba(56, 189, 248, .14); }
    .password-wrap { position: relative; }
    .password-wrap input { padding-right: 56px; }
    .toggle {
      position: absolute; right: 9px; top: 8px; height: 34px; padding: 0 8px;
      border: 0; border-radius: 8px; color: #9fb1c7; background: transparent;
      font: inherit; cursor: pointer;
    }
    .toggle:hover { color: #e5edf8; background: #1e293b; }
    .submit {
      width: 100%; height: 50px; margin-top: 8px; border: 0; border-radius: 12px;
      color: white; font: inherit; font-weight: 700; cursor: pointer;
      background: linear-gradient(100deg, #0284c7, #4f46e5);
      box-shadow: 0 10px 25px rgba(37, 99, 235, .22); transition: filter .2s, transform .2s;
    }
    .submit:hover { filter: brightness(1.12); transform: translateY(-1px); }
    .submit:disabled { opacity: .65; cursor: wait; transform: none; }
    .error {
      display: none; margin: 0 0 18px; padding: 11px 13px;
      border: 1px solid rgba(248, 113, 113, .32); border-radius: 10px;
      color: #fecaca; background: rgba(127, 29, 29, .2); font-size: 14px;
    }
    .security-note {
      margin-top: 24px; text-align: center; color: #718198; font-size: 12px;
    }
    @media (max-width: 480px) {
      body { padding: 16px; }
      .login-card { padding: 30px 23px; border-radius: 20px; }
      h1 { font-size: 25px; }
    }
  </style>
</head>
<body>
  <main class="login-card">
    <div class="brand">
      <div class="brand-icon" aria-hidden="true">&#8962;</div>
      <div><div class="brand-name">SMART HOME</div><strong>Control Panel</strong></div>
    </div>
    <h1>Welcome back</h1>
    <p class="subtitle">Sign in to manage your devices, rooms, and system settings.</p>
    <div id="loginError" class="error" role="alert"></div>
    <form id="loginForm">
      <div class="field">
        <label for="username">Username</label>
        <input id="username" name="username" type="text" autocomplete="username"
               placeholder="Enter your username" required autofocus>
      </div>
      <div class="field">
        <label for="password">Password</label>
        <div class="password-wrap">
          <input id="password" name="password" type="password"
                 autocomplete="current-password" placeholder="Enter your password" required>
          <button class="toggle" type="button" id="togglePassword" aria-label="Show password">Show</button>
        </div>
      </div>
      <button class="submit" id="submitButton" type="submit">Sign in</button>
    </form>
    <div class="security-note">Secure access to your home dashboard</div>
  </main>
  <script>
    const form = document.getElementById("loginForm");
    const errorBox = document.getElementById("loginError");
    const submitButton = document.getElementById("submitButton");
    const passwordInput = document.getElementById("password");
    document.getElementById("togglePassword").addEventListener("click", function () {
      const visible = passwordInput.type === "password";
      passwordInput.type = visible ? "text" : "password";
      this.textContent = visible ? "Hide" : "Show";
      this.setAttribute("aria-label", visible ? "Hide password" : "Show password");
    });
    form.addEventListener("submit", async function (event) {
      event.preventDefault();
      errorBox.style.display = "none";
      submitButton.disabled = true;
      submitButton.textContent = "Signing in...";
      try {
        const response = await fetch("/api/login", {
          method: "POST",
          headers: { "Content-Type": "application/x-www-form-urlencoded" },
          body: new URLSearchParams(new FormData(form))
        });
        const message = await response.text();
        if (!response.ok) throw new Error(message || "Unable to sign in");
        window.location.assign("/");
      } catch (error) {
        errorBox.textContent = error.message;
        errorBox.style.display = "block";
        submitButton.disabled = false;
        submitButton.textContent = "Sign in";
      }
    });
  </script>
</body>
</html>
)rawliteral");
    }
};
