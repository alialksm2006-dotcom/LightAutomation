#pragma once

#include <Arduino.h>
#include <esp_system.h>
#include "EspServer.h"

class WebAuth
{
private:
    static String sessionToken;
    static uint32_t sessionStarted;
    static const uint32_t sessionLifetimeMs = 8UL * 60UL * 60UL * 1000UL;
    static const char *cookieName()
    {
        return "esp_session";
    }

public:
    static const char *username()
    {
        return "admin";
    }

    static const char *password()
    {
        return "LightHub2026!";
    }

    static bool isAuthenticated()
    {
        if (sessionToken.isEmpty() ||
            static_cast<uint32_t>(millis() - sessionStarted) >= sessionLifetimeMs)
        {
            sessionToken = "";
            return false;
        }

        String cookies = EspServer::server.header("Cookie");
        int start = 0;
        while (start < cookies.length())
        {
            int end = cookies.indexOf(';', start);
            if (end < 0)
                end = cookies.length();
            String cookie = cookies.substring(start, end);
            cookie.trim();
            String expected = String(cookieName()) + "=" + sessionToken;
            if (cookie == expected)
                return true;
            start = end + 1;
        }
        return false;
    }

    static void login()
    {
        if (!EspServer::server.hasArg("username") ||
            !EspServer::server.hasArg("password"))
        {
            EspServer::server.send(400, "text/plain", "Username and password are required");
            return;
        }

        const String providedUsername = EspServer::server.arg("username");
        const String providedPassword = EspServer::server.arg("password");
        if (providedUsername != username() || providedPassword != password())
        {
            EspServer::server.send(401, "text/plain", "Incorrect username or password");
            return;
        }

        char token[33];
        snprintf(token, sizeof(token), "%08lx%08lx%08lx%08lx",
                 static_cast<unsigned long>(esp_random()),
                 static_cast<unsigned long>(esp_random()),
                 static_cast<unsigned long>(esp_random()),
                 static_cast<unsigned long>(esp_random()));
        sessionToken = token;
        sessionStarted = millis();

        EspServer::server.sendHeader(
            "Set-Cookie",
            String(cookieName()) + "=" + sessionToken +
                "; Path=/; HttpOnly; Secure; SameSite=Strict; Max-Age=28800");
        EspServer::server.send(200, "text/plain", "Login successful");
    }

    static void logout()
    {
        sessionToken = "";
        sessionStarted = 0;
        EspServer::server.sendHeader(
            "Set-Cookie",
            String(cookieName()) + "=; Path=/; HttpOnly; Secure; SameSite=Strict; Max-Age=0");
        EspServer::server.send(200, "text/plain", "Logged out");
    }

    static void rejectRequest()
    {
        EspServer::server.sendHeader("Cache-Control", "no-store");
        EspServer::server.send(401, "text/plain", "Authentication required");
    }
};
