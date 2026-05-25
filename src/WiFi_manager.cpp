#include "WiFi_manager.h"

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#include "config.h"
#include "ui_state.h"

#include "WiFi_storage.h"
#include "WiFi_web.h"

/* =========================================================
   WIFI STATE
========================================================= */

enum wifi_state_t
{
    WIFI_BOOT,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_AP_MODE
};

static wifi_state_t wifi_state =
    WIFI_BOOT;

/* =========================================================
   GLOBALS
========================================================= */

static bool ap_started = false;

static unsigned long wifi_start_ms = 0;
static unsigned long ap_start_ms = 0;
static unsigned long last_ntp_sync_ms = 0;
static bool ntp_done = false;

/* =========================================================
   RUNTIME CACHE
========================================================= */

static char runtime_ssid[64]  = {0};
static char runtime_passw[64] = {0};

/* =========================================================
   WIFI STA START
========================================================= */

static void wifi_start_sta()
{
    memset(
        runtime_ssid,
        0,
        sizeof(runtime_ssid)
    );

    memset(
        runtime_passw,
        0,
        sizeof(runtime_passw)
    );

    /*
       load runtime credentials
    */

    loadWiFiCredentials(
        runtime_ssid,
        runtime_passw
    );

    /*
       fallback:
       use first saved profile
    */

    if (strlen(runtime_ssid) == 0)
    {
        WiFiProfileDyn p;

        if (loadProfile(0, p))
        {
            strlcpy(
                runtime_ssid,
                p.ssid.c_str(),
                sizeof(runtime_ssid)
            );

            strlcpy(
                runtime_passw,
                p.passw.c_str(),
                sizeof(runtime_passw)
            );
        }
    }

    Serial.print(
        "[WIFI] connecting to: "
    );

    Serial.println(runtime_ssid);

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        runtime_ssid,
        runtime_passw
    );

    wifi_state =
        WIFI_CONNECTING;

    wifi_start_ms =
        millis();
}

/* =========================================================
   WIFI SCAN CACHE
========================================================= */

static void wifi_scan_cache()
{
    Serial.println(
        "[WIFI] scanning networks..."
    );

    clearScanResults();

    /*
       stop previous STA
    */

    WiFi.disconnect(true);

    delay(500);

    /*
       force clean STA mode
    */

    WiFi.mode(WIFI_STA);

    delay(1000);

    /*
       clear old scan
    */

    WiFi.scanDelete();

    /*
       blocking scan
    */

    int n =
        WiFi.scanNetworks();

    Serial.printf(
        "[WIFI] found %d networks\n",
        n
    );

    if (n <= 0)
    {
        return;
    }

    for (int i = 0;
         i < n;
         i++)
    {
        String ssid =
            WiFi.SSID(i);

        if (!ssid.length())
            continue;

        addScanResult(
            ssid,
            WiFi.RSSI(i)
        );
    }

    WiFi.scanDelete();
}

/* =========================================================
   WIFI AP START
========================================================= */

static void wifi_start_ap()
{
    if (ap_started)
        return;

    WiFi.mode(WIFI_AP_STA);

    delay(100);

    WiFi.softAP(
        WIFI_AP_SSID
    );

    startWiFiWebServer();

    q8_enter_ap_mode(
        WiFi.softAPIP()
            .toString()
            .c_str(),

        "OPEN"
    );

    ap_started = true;

    ap_start_ms =
        millis();

    Serial.println(
        "[WIFI] AP mode started"
    );
}

/* =========================================================
   WIFI AP STOP
========================================================= */

static void wifi_stop_ap()
{
    if (!ap_started)
        return;

    WiFi.softAPdisconnect(true);

    ap_started = false;

    Serial.println(
        "[WIFI] AP stopped"
    );
}

/* =========================================================
   AP TIMEOUT
========================================================= */

static void handle_ap_timeout()
{
    if (!ap_started)
        return;

    if (
        millis() - ap_start_ms >
        (15UL * 60UL * 1000UL)
    )
    {
        Serial.println(
            "[WIFI] AP timeout reboot"
        );

        ESP.restart();
    }
}

/* =========================================================
   NTP
========================================================= */

static void handle_ntp()
{
    if (wifi_state != WIFI_CONNECTED)
        return;

    if (!ntp_done)
    {
        configTime(
            GMT_OFFSET_SEC,
            DAYLIGHT_OFFSET_SEC,
            NTP_SERVER
        );

        ntp_done = true;

        last_ntp_sync_ms =
            millis();

        Serial.println(
            "[NTP] synchronized"
        );
    }

    if (
        millis() - last_ntp_sync_ms >
        (24UL * 60UL * 60UL * 1000UL)
    )
    {
        configTime(
            GMT_OFFSET_SEC,
            DAYLIGHT_OFFSET_SEC,
            NTP_SERVER
        );

        last_ntp_sync_ms =
            millis();

        Serial.println(
            "[NTP] resync"
        );
    }
}

/* =========================================================
   INIT WIFI
========================================================= */

void initWiFi()
{
    initDefaultProfiles();

    wifi_state =
        WIFI_BOOT;
}

/* =========================================================
   UPDATE WIFI
========================================================= */

void updateWiFi()
{
    /*
       handle web ui
    */

    if (ap_started)
    {
        handleWiFiWebServer();

        handle_ap_timeout();
    }

    /*
       ntp
    */

    handle_ntp();

    /*
       fsm
    */

    switch (wifi_state)
    {
        /* =============================================
           BOOT
        ============================================= */

        case WIFI_BOOT:

            wifi_start_sta();

            break;

        /* =============================================
           CONNECTING
        ============================================= */

        case WIFI_CONNECTING:

            if (
                WiFi.status() ==
                WL_CONNECTED
            )
            {
                Serial.println(
                    "[WIFI] connected"
                );

                Serial.println(
                    WiFi.localIP()
                );

                wifi_state =
                    WIFI_CONNECTED;
            }
            else if (
                millis() - wifi_start_ms >
                WIFI_TIMEOUT_MS
            )
            {
                Serial.println(
                    "[WIFI] timeout");
            wifi_scan_cache();
                 wifi_state =
                    WIFI_AP_MODE;
            }

            break;

        /* =============================================
           CONNECTED
        ============================================= */

        case WIFI_CONNECTED:

            wifi_stop_ap();

            break;

        /* =============================================
           AP MODE
        ============================================= */

        case WIFI_AP_MODE:
            
            wifi_start_ap();

            break;
    }
}

/* =========================================================
   STATUS HELPERS
========================================================= */

bool isWiFiConnected()
{
    return (
        WiFi.status() ==
        WL_CONNECTED
    );
}

bool isAPMode()
{
    return (
        wifi_state ==
        WIFI_AP_MODE
    );
}