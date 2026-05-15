#pragma once

#include <Arduino.h>

/* =========================================================
   PROFILE STRUCT
========================================================= */

typedef struct
{
    String name;
    String ssid;
    String passw;
}
WiFiProfileDyn;

/* =========================================================
   RUNTIME WIFI
========================================================= */

void loadWiFiCredentials(
    char* ssid,
    char* passw
);

void saveWiFiCredentials(
    const char* ssid,
    const char* passw
);

bool hasWiFiCredentials();

/* =========================================================
   PROFILE DATABASE
========================================================= */

uint8_t getProfileCount();

bool loadProfile(
    uint8_t index,
    WiFiProfileDyn& profile
);

bool saveProfile(
    const String& name,
    const String& ssid,
    const String& passw
);

bool deleteProfile(
    uint8_t index
);

void initDefaultProfiles();