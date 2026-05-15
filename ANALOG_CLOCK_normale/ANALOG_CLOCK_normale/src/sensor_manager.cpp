#include "sensor_manager.h"
#include <Arduino.h>
#include <DHT.h>
#include "config.h"
#include "calibration.h"
/* =========================
   OGGETTI / VARIABILI
   ========================= */
static DHT dht(DHT_PIN, DHT_TYPE);

static float temperature = 0.0f;
static float humidity = 0.0f;

static unsigned long last_read_ms = 0;

/* =========================
   INIT
   ========================= */
void initSensor()
{
    dht.begin();
    last_read_ms = millis() - 3000; // prima lettura immediata
}

/* =========================
   UPDATE
   ========================= */
void updateSensor()
{
    if (millis() - last_read_ms < 3000) return;

    last_read_ms = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h))
    {
        temperature = t + TEMP_OFFSET_DHT11;
        humidity = h + HUM_OFFSET_DHT11;
    }
}

/* =========================
   GETTERS
   ========================= */
float getTemperature()
{
    return temperature;
}

float getHumidity()
{
    return humidity;
}