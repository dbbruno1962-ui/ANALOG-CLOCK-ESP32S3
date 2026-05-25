#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "esp_heap_caps.h"
#include "src/config.h"
#include "src/WiFi_manager.h"
#include "src/ntp_manager.h"
#include "src/sensor_manager.h"
#include "src/ui_state.h"
#include "src/net_manager.h"
#include "src/camera_fw.h"

/* =========================
   DISPLAY
   ========================= */
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    LCD_DC, LCD_CS, SPI_SCLK, SPI_MOSI, SPI_MISO);

Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, LCD_RST, 1, false);

/* =========================
   LVGL
   ========================= */
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
static lv_disp_drv_t disp_drv;

/* =========================
   FLUSH
   ========================= */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    gfx->draw16bitRGBBitmap(
        area->x1,
        area->y1,
        (uint16_t *)color_p,
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1);

    lv_disp_flush_ready(disp);
}

/* =========================
   SETUP
   ========================= */
void setup()
{
    Serial.begin(115200);

    gfx->begin();
    lv_init();

    initWiFi();
    initTime();
    initSensor();

    buf = (lv_color_t *)heap_caps_malloc(
        320 * 20 * sizeof(lv_color_t),
        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 20);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    ui_init();
}

void loop()
{
    lv_timer_handler();
    delay(5);

    updateWiFi();   // gestisce stato WiFi

    net_update();   // polling (solo se connesso)
    cameraHandle(); // solo se attiva

    // 🔥 UI gestita UNA sola volta
    ui_update();
}