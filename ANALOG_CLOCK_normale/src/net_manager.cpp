#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include "camera_fw.h"
#include "WiFi_manager.h"
#include "config.h"

static unsigned long last_poll_ms = 0;

#define POLL_IDLE_MS    5000UL
#define POLL_ACTIVE_MS  3000UL

// Endpoint pubblico attuale

// Flag resolve inviato
static bool endpoint_sent = false;

/* =====================================
   SEND ENDPOINT TO SERVER
===================================== */

void sendEndpointToServer()
{
    if (!isWiFiConnected()) return;

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setTimeout(5000);

    String url =
    "https://www.giocando.net/emilia/resolve.php?id=esp_0aef2c";

    http.begin(client, url);

    http.setAuthorization(poll_name, poll_psw);

    int code = http.GET();

    Serial.print("[RESOLVE] HTTP: ");
    Serial.println(code);

    http.end();
}

/* =====================================
   NETWORK UPDATE
===================================== */

void net_update()
{
    if (!isWiFiConnected()) {
        endpoint_sent = false;
        return;
    }

    // Invia endpoint una sola volta dopo connessione WiFi
    if (!endpoint_sent)
    {
        sendEndpointToServer();
        endpoint_sent = true;
    }

    unsigned long now = millis();

    unsigned long interval = cameraIsActive() ? POLL_ACTIVE_MS
                                              : POLL_IDLE_MS;

    if (now - last_poll_ms < interval) return;

    last_poll_ms = now;

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setTimeout(5000);

    http.begin(client, "https://www.giocando.net/emilia/idle.php");

    http.setAuthorization(poll_name, poll_psw);

    int code = http.GET();

    if (code == 200)
    {
        String cmd = http.getString();
        cmd.trim();

        Serial.print("[POLL] CMD: ");
        Serial.println(cmd);

        if (cmd == "STARTCAM")
        {
            if (!cameraIsActive())
            {
                cameraStart();
            }
        }

        if (cmd == "STOPCAM")
        {
            if (cameraIsActive())
            {
                cameraStop();
            }
        }
    }
    else
    {
        Serial.print("[POLL] HTTP ERROR: ");
        Serial.println(code);
    }

    http.end();
}