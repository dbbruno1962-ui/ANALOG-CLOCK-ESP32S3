#include "WiFi_storage.h"

#include <Preferences.h>

#include "wifi_profiles.h"

/* =========================================================
   PREFERENCES
========================================================= */

static Preferences wifi_prefs;
static Preferences profile_prefs;

/* =========================================================
   PROFILE KEY HELPER
========================================================= */

static String makeProfileKey(
    uint8_t index,
    const char* field)
{
    char key[24];

    snprintf(
        key,
        sizeof(key),
        "p%d_%s",
        index,
        field
    );

    return String(key);
}

/* =========================================================
   LOAD RUNTIME WIFI
========================================================= */

void loadWiFiCredentials(
    char* ssid,
    char* passw)
{
    wifi_prefs.begin(
        "wifi",
        true
    );

    String s =
        wifi_prefs.getString(
            "ssid",
            ""
        );

    String p =
        wifi_prefs.getString(
            "passw",
            ""
        );

    wifi_prefs.end();

    strlcpy(
        ssid,
        s.c_str(),
        64
    );

    strlcpy(
        passw,
        p.c_str(),
        64
    );
}

/* =========================================================
   SAVE RUNTIME WIFI
========================================================= */

void saveWiFiCredentials(
    const char* ssid,
    const char* passw)
{
    wifi_prefs.begin(
        "wifi",
        false
    );

    wifi_prefs.putString(
        "ssid",
        ssid
    );

    wifi_prefs.putString(
        "passw",
        passw
    );

    wifi_prefs.end();
}

/* =========================================================
   HAS WIFI CREDENTIALS
========================================================= */

bool hasWiFiCredentials()
{
    wifi_prefs.begin(
        "wifi",
        true
    );

    String s =
        wifi_prefs.getString(
            "ssid",
            ""
        );

    wifi_prefs.end();

    return (
        s.length() > 0
    );
}

/* =========================================================
   PROFILE COUNT
========================================================= */

uint8_t getProfileCount()
{
    profile_prefs.begin(
        "profiles",
        true
    );

    uint8_t count =
        profile_prefs.getUChar(
            "count",
            0
        );

    profile_prefs.end();

    return count;
}

/* =========================================================
   SAVE PROFILE
========================================================= */

bool saveProfile(
    const String& name,
    const String& ssid,
    const String& passw)
{
    profile_prefs.begin(
        "profiles",
        false
    );

    uint8_t count =
        profile_prefs.getUChar(
            "count",
            0
        );

    profile_prefs.putString(
        makeProfileKey(count, "name").c_str(),
        name
    );

    profile_prefs.putString(
        makeProfileKey(count, "ssid").c_str(),
        ssid
    );

    profile_prefs.putString(
        makeProfileKey(count, "passw").c_str(),
        passw
    );

    profile_prefs.putUChar(
        "count",
        count + 1
    );

    profile_prefs.end();

    return true;
}

/* =========================================================
   LOAD PROFILE
========================================================= */

bool loadProfile(
    uint8_t index,
    WiFiProfileDyn& profile)
{
    profile_prefs.begin(
        "profiles",
        true
    );

    uint8_t count =
        profile_prefs.getUChar(
            "count",
            0
        );

    if (index >= count)
    {
        profile_prefs.end();

        return false;
    }

    profile.name =
        profile_prefs.getString(
            makeProfileKey(index, "name").c_str(),
            ""
        );

    profile.ssid =
        profile_prefs.getString(
            makeProfileKey(index, "ssid").c_str(),
            ""
        );

    profile.passw =
        profile_prefs.getString(
            makeProfileKey(index, "passw").c_str(),
            ""
        );

    profile_prefs.end();

    return true;
}

/* =========================================================
   DELETE PROFILE
========================================================= */

bool deleteProfile(
    uint8_t index)
{
    profile_prefs.begin(
        "profiles",
        false
    );

    uint8_t count =
        profile_prefs.getUChar(
            "count",
            0
        );

    if (index >= count)
    {
        profile_prefs.end();

        return false;
    }

    for (uint8_t i = index;
         i < count - 1;
         i++)
    {
        String next_name =
            profile_prefs.getString(
                makeProfileKey(i + 1, "name").c_str(),
                ""
            );

        String next_ssid =
            profile_prefs.getString(
                makeProfileKey(i + 1, "ssid").c_str(),
                ""
            );

        String next_passw =
            profile_prefs.getString(
                makeProfileKey(i + 1, "passw").c_str(),
                ""
            );

        profile_prefs.putString(
            makeProfileKey(i, "name").c_str(),
            next_name
        );

        profile_prefs.putString(
            makeProfileKey(i, "ssid").c_str(),
            next_ssid
        );

        profile_prefs.putString(
            makeProfileKey(i, "passw").c_str(),
            next_passw
        );
    }

    uint8_t last =
        count - 1;

    profile_prefs.remove(
        makeProfileKey(last, "name").c_str()
    );

    profile_prefs.remove(
        makeProfileKey(last, "ssid").c_str()
    );

    profile_prefs.remove(
        makeProfileKey(last, "passw").c_str()
    );

    profile_prefs.putUChar(
        "count",
        last
    );

    profile_prefs.end();

    return true;
}

/* =========================================================
   IMPORT DEFAULT PROFILES
========================================================= */

void initDefaultProfiles()
{
    if (getProfileCount() > 0)
        return;

    for (uint8_t i = 0;
         i < WIFI_PROFILE_COUNT;
         i++)
    {
        saveProfile(
            wifi_profiles[i].profile_name,
            wifi_profiles[i].ssid,
            wifi_profiles[i].passw
        );
    }
}