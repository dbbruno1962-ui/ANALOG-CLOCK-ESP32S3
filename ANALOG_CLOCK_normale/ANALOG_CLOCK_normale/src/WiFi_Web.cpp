// WiFi_web.cpp

#include "WiFi_web.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "WiFi_storage.h"

/* =========================================================
   WEB SERVER
========================================================= */

static WebServer server(80);

/* =========================================================
   SCAN CACHE
========================================================= */

typedef struct
{
    String ssid;
    int32_t rssi;
}
ScanResult;

#define MAX_SCAN_RESULTS 20

static ScanResult scan_results[
    MAX_SCAN_RESULTS
];

static uint8_t scan_count = 0;

/* =========================================================
   SCAN HELPERS
========================================================= */

void clearScanResults()
{
    scan_count = 0;
}

void addScanResult(
    const String& ssid,
    int32_t rssi)
{
    if (scan_count >=
        MAX_SCAN_RESULTS)
    {
        return;
    }

    scan_results[scan_count].ssid =
        ssid;

    scan_results[scan_count].rssi =
        rssi;

    scan_count++;
}

/* =========================================================
   HTML PAGE
========================================================= */

static String makeHomePage()
{
    String html;

    html +=
        "<html>"
        "<head>"

        "<meta name='viewport' "
        "content='width=device-width'>"

        "<style>"

        ":root{"
        "--bg:#111;"
        "--card:#222;"
        "--text:#eee;"
        "--link:#6cf;"
        "}"

        "@media "
        "(prefers-color-scheme: light)"
        "{"

        ":root{"
        "--bg:#f2f2f2;"
        "--card:#ffffff;"
        "--text:#111;"
        "--link:#0066cc;"
        "}"

        "}"

        "body{"
        "background:var(--bg);"
        "color:var(--text);"
        "font-family:Arial;"
        "padding:12px;"
        "}"

        ".card{"
        "background:var(--card);"
        "padding:12px;"
        "border-radius:12px;"
        "margin-bottom:12px;"
        "}"

        "button{"
        "width:100%;"
        "padding:14px;"
        "margin-top:8px;"
        "font-size:16px;"
        "border-radius:10px;"
        "border:none;"
        "}"

        "input{"
        "width:100%;"
        "padding:12px;"
        "margin-top:8px;"
        "font-size:16px;"
        "}"

        "a{"
        "color:var(--link);"
        "}"

        "</style>"

        "</head>"
        "<body>";

    /* =====================================================
       STATUS
    ===================================================== */

    html +=
        "<div class='card'>";

    html +=
        "<h2>Clock WiFi Manager</h2>";

    html +=
        "Status: ";

    html +=
        (WiFi.status() == WL_CONNECTED)
        ? "CONNECTED"
        : "DISCONNECTED";

    html +=
        "<br><br>";

    html +=
        "IP:<br>";

    html +=
        WiFi.localIP().toString();

    html +=
        "<br><br>";

    html +=
        "RSSI:<br>";

    html +=
        String(WiFi.RSSI());

    html +=
        "</div>";

    /* =====================================================
       SAVED PROFILES
    ===================================================== */

    html +=
        "<div class='card'>";

    html +=
        "<h3>Saved Profiles</h3>";

    uint8_t count =
        getProfileCount();

    for (uint8_t i = 0;
         i < count;
         i++)
    {
        WiFiProfileDyn p;

        if (!loadProfile(i, p))
            continue;

        html +=
            "<b>";

        html += p.name;

        html +=
            "</b><br>";

        html += p.ssid;

        html +=
            "<br><br>";

        html +=
            "<a href='/setruntime?id=";

        html += i;

        html +=
            "'>SET RUNTIME</a>";

        html +=
            " | ";

        html +=
            "<a href='/delete?id=";

        html += i;

        html +=
            "'>DELETE</a>";

        html +=
            "<hr>";
    }

    html +=
        "</div>";

    /* =====================================================
       SCAN RESULTS
    ===================================================== */

    html +=
        "<div class='card'>";

    html +=
        "<h3>Available Networks</h3>";

    if (scan_count == 0)
    {
        html +=
            "No networks found";
    }
    else
    {
        for (uint8_t i = 0;
             i < scan_count;
             i++)
        {
            html +=
                "<b>";

            html +=
                scan_results[i].ssid;

            html +=
                "</b>";

            html +=
                " (";

            html +=
                String(
                    scan_results[i].rssi
                );

            html +=
                ")";

            html +=
                "<br><br>";
        }
    }

    html +=
        "</div>";

    /* =====================================================
       NEW PROFILE
    ===================================================== */

    html +=
        "<div class='card'>"

        "<h3>New Profile</h3>"

        "<form action='/save'>"

        "Profile Name"

        "<input name='name'>"

        "SSID"

        "<input name='ssid'>"

        "Password"

        "<input "
        "name='passw' "
        "type='password'>"

        "<button>"
        "SAVE PROFILE"
        "</button>"

        "</form>"

        "</div>";

    html +=
        "</body>"
        "</html>";

    return html;
}

/* =========================================================
   START WEB SERVER
========================================================= */

void startWiFiWebServer()
{
    /* =====================================================
       HOME
    ===================================================== */

    server.on("/", HTTP_GET, []()
    {
        server.send(
            200,
            "text/html",
            makeHomePage()
        );
    });

    /* =====================================================
       SAVE PROFILE
    ===================================================== */

    server.on("/save", HTTP_GET, []()
    {
        if (!server.hasArg("ssid"))
        {
            server.send(
                400,
                "text/plain",
                "Missing SSID"
            );

            return;
        }

        saveProfile(
            server.arg("name"),
            server.arg("ssid"),
            server.arg("passw")
        );

        server.sendHeader(
            "Location",
            "/"
        );

        server.send(302);
    });

    /* =====================================================
       SET RUNTIME
    ===================================================== */

    server.on("/setruntime", HTTP_GET, []()
    {
        if (!server.hasArg("id"))
        {
            server.send(
                400,
                "text/plain",
                "Missing ID"
            );

            return;
        }

        uint8_t id =
            server.arg("id").toInt();

        WiFiProfileDyn p;

        if (!loadProfile(id, p))
        {
            server.send(
                404,
                "text/plain",
                "Invalid profile"
            );

            return;
        }

        saveWiFiCredentials(
            p.ssid.c_str(),
            p.passw.c_str()
        );

        server.send(
            200,
            "text/html",
            "Runtime updated.<br>"
            "Rebooting..."
        );

        delay(1000);

        ESP.restart();
    });

    /* =====================================================
       DELETE PROFILE
    ===================================================== */

    server.on("/delete", HTTP_GET, []()
    {
        if (!server.hasArg("id"))
        {
            server.send(
                400,
                "text/plain",
                "Missing ID"
            );

            return;
        }

        deleteProfile(
            server.arg("id").toInt()
        );

        server.sendHeader(
            "Location",
            "/"
        );

        server.send(302);
    });

    server.begin();

    Serial.println(
        "[WEB] started"
    );
}

/* =========================================================
   HANDLE WEB SERVER
========================================================= */

void handleWiFiWebServer()
{
    server.handleClient();
}
