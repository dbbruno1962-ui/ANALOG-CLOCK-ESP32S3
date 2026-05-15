#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include "camera_fw.h"
#include "WiFi_manager.h"
#include "config.h"

static unsigned long last_poll_ms = 0;

#define POLL_IDLE_MS    5000UL
#define POLL_ACTIVE_MS  3000UL

/* =====================================
   URLS
===================================== */

static bool endpoint_sent = false;

static char idle_url[128];
static char resolve_url[128];

/* =====================================
   INIT URLS
===================================== */

static void initNetURLs()
{
    snprintf(
        idle_url,
        sizeof(idle_url),
        "https://www.giocando.net/%s/idle.php",
        USER_FIN
    );

    snprintf(
        resolve_url,
        sizeof(resolve_url),
        "https://www.giocando.net/%s/resolve.php?id=esp_0aef2c",
        USER_FIN
    );

    Serial.print("[NET] idle: ");
    Serial.println(idle_url);

    Serial.print("[NET] resolve: ");
    Serial.println(resolve_url);
}

/* =====================================
   SEND ENDPOINT TO SERVER
===================================== */

void sendEndpointToServer()
{
    if (!isWiFiConnected())
        return;

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setTimeout(5000);

    http.begin(client, resolve_url);

    http.setAuthorization(
        poll_name,
        poll_psw
    );

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
    static bool init_done = false;

    if (!init_done)
    {
        initNetURLs();
        init_done = true;
    }

    if (!isWiFiConnected())
    {
        endpoint_sent = false;
        return;
    }

    /* =====================================
       SEND ENDPOINT ONCE
    ===================================== */

    if (!endpoint_sent)
    {
        sendEndpointToServer();
        endpoint_sent = true;
    }

    unsigned long now = millis();

    unsigned long interval =
        cameraIsActive()
        ? POLL_ACTIVE_MS
        : POLL_IDLE_MS;

    if (now - last_poll_ms < interval)
        return;

    last_poll_ms = now;

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setTimeout(5000);

    http.begin(client, idle_url);

    http.setAuthorization(
        poll_name,
        poll_psw
    );

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