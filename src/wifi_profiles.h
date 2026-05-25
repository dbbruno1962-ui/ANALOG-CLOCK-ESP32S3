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
        "Vodafone-48402720",
        "Alligatore1"
    },

    {
        "Android",
        "BrunoPhone",
        "Alligatore1"
    },

    {
        "Hotspot",
        "G4-CPE-7B65",
        "12345678"
    }
};

static const uint8_t WIFI_PROFILE_COUNT =
    sizeof(wifi_profiles) /
    sizeof(wifi_profiles[0]);