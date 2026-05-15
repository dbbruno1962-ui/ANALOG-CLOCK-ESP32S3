#include "ntp_manager.h"

#include <Arduino.h>
#include <time.h>
#include "config.h"

/* =========================
   STATO
   ========================= */
static unsigned long last_ntp_sync_ms = 0;

/* =========================
   INIT
   ========================= */
void initTime()
{
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

/* =========================
   SYNC
   ========================= */
bool syncTime(int16_t &hh, int16_t &mm, int16_t &ss)
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo))
    {
        return false;
    }

    hh = timeinfo.tm_hour;
    mm = timeinfo.tm_min;
    ss = timeinfo.tm_sec;

    last_ntp_sync_ms = millis();
    return true;
}

/* =========================
   RESYNC CHECK
   ========================= */
bool needResync()
{
    return millis() - last_ntp_sync_ms > (24UL * 60UL * 60UL * 1000UL);
}