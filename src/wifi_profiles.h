#pragma once

typedef struct
{
    const char* profile_name;
    const char* ssid;
    const char* passw;
}
WiFiProfile;

static const WiFiProfile wifi_profiles[] =
{
    {
        "Casa",
        "YourSSID",
        "YourPASSW"
    },

    {
        "Android",
        "YourSSID",
        "YourPASSW"
    },

    {
        "Hotspot",
        "YourSSID",
        "YourPASSW"
    }
};

static const uint8_t WIFI_PROFILE_COUNT =
    sizeof(wifi_profiles) /
    sizeof(wifi_profiles[0]);
