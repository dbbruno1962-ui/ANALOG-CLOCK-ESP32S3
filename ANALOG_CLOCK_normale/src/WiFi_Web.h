#pragma once

#include <Arduino.h>

/* =========================================================
   WEB SERVER
========================================================= */

void startWiFiWebServer();

void handleWiFiWebServer();

/* =========================================================
   SCAN CACHE
========================================================= */

void clearScanResults();

void addScanResult(
    const String& ssid,
    int32_t rssi
);
