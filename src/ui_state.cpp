#include "ui_state.h"
#include "sensor_manager.h"

#include <Arduino.h>
#include <lvgl.h>
#include <math.h>

#include "lancette.h"
#include "clock_day.h"
#include "clock_night.h"

/* ========================= */
#define CENTER_X 160
#define CENTER_Y 160
/* ========================= */

/* UI */
static lv_obj_t *img_clock;
static lv_obj_t *img_hour;
static lv_obj_t *img_min;

/* labels */
static lv_obj_t *label_sec;
static lv_obj_t *label_time;
static lv_obj_t *label_info;
static lv_obj_t *label_day;
static lv_obj_t *label_date;
static lv_obj_t *label_year;
static lv_obj_t *label_wifi;
/* Q8 state */
static bool q8_active = false;

/* Theme */
static bool night_mode = true;

/* ========================= */
bool isQ8Active(void)
{
    return q8_active;
}

/* =========================
   THEME SWITCH
   ========================= */
static void update_theme(int hour)
{
    bool new_night =
        (hour >= 19 || hour < 6);

    if (new_night == night_mode)
        return;

    night_mode = new_night;

    if (night_mode)
    {
        lv_img_set_src(img_clock, &clock_night);

        lv_obj_set_style_bg_color(
            lv_scr_act(),
            lv_color_black(),
            0
        );
    }
    else
    {
        lv_img_set_src(img_clock, &clock_day);

        lv_obj_set_style_bg_color(
            lv_scr_act(),
           lv_color_black(),
            0
        );
    }
}

/* =========================
   LANCETTE
   ========================= */
static void update_hands(int h, int m, int s)
{
    if (!img_hour || !img_min) return;

    int angle_h =
        (h % 12 + m / 60.0) * 30 * 10;

    int angle_m =
        (m + s / 60.0) * 6 * 10;

    lv_img_set_angle(img_hour, angle_h);
    lv_img_set_angle(img_min, angle_m);
}

/* =========================
   UI INIT
   ========================= */
void ui_init(void)
{
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    /* QUADRANTE */
    img_clock = lv_img_create(lv_scr_act());

    lv_img_set_src(
        img_clock,
        &clock_night
    );

    lv_obj_align(
        img_clock,
        LV_ALIGN_LEFT_MID,
        0,
        0
    );

    /* ORE */
    img_hour = lv_img_create(lv_scr_act());

    lv_img_set_src(
        img_hour,
        &lancetta_ore
    );

    lv_img_set_pivot(
        img_hour,
        159,
        159
    );

    lv_obj_set_pos(
        img_hour,
        CENTER_X - 159,
        CENTER_Y - 159
    );

    /* MINUTI */
    img_min = lv_img_create(lv_scr_act());

    lv_img_set_src(
        img_min,
        &lancetta_minuti
    );

    lv_img_set_pivot(
        img_min,
        159,
        159
    );

    lv_obj_set_pos(
        img_min,
        CENTER_X - 159,
        CENTER_Y - 159
    );

    update_hands(10, 10, 0);

    /* COLONNA DX */
    int x = 330;
    int y = 10;

    label_sec = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        label_sec,
        &lv_font_montserrat_48,
        0
    );

    lv_obj_set_style_text_color(
        label_sec,
        lv_palette_main(LV_PALETTE_RED),
        0
    );

    lv_obj_set_pos(label_sec, x, y);

label_wifi = lv_label_create(lv_scr_act());

lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);

lv_obj_set_style_text_font(
    label_wifi,
    &lv_font_montserrat_28,
    0
);

lv_obj_set_style_text_color(
    label_wifi,
    lv_palette_main(LV_PALETTE_GREEN),
    0
);

lv_obj_set_pos(label_wifi, 420, 20);

    y += 60;

    label_time = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        label_time,
        &lv_font_montserrat_32,
        0
    );

    lv_obj_set_style_text_color(
        label_time,
        lv_palette_main(LV_PALETTE_YELLOW),
        0
    );

    lv_obj_set_pos(label_time, x, y);

    y += 45;

    label_info = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        label_info,
        &lv_font_montserrat_32,
        0
    );

    lv_obj_set_style_text_color(
        label_info,
        lv_palette_main(LV_PALETTE_GREEN),
        0
    );

    lv_obj_set_pos(label_info, x, y);

    y += 75;

    label_day = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        label_day,
        &lv_font_montserrat_32,
        0
    );

    lv_obj_set_style_text_color(
        label_day,
        lv_palette_main(LV_PALETTE_YELLOW),
        0
    );

    lv_obj_set_pos(label_day, x, y);

    y += 35;

    label_date = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        label_date,
        &lv_font_montserrat_30,
        0
    );

    lv_obj_set_style_text_color(
        label_date,
        lv_palette_main(LV_PALETTE_BLUE),
        0
    );

    lv_obj_set_pos(label_date, x, y);

    y += 35;

    label_year = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        label_year,
        &lv_font_montserrat_30,
        0
    );

    lv_obj_set_style_text_color(
        label_year,
        lv_palette_main(LV_PALETTE_BLUE),
        0
    );

    lv_obj_set_pos(label_year, x, y);
}

/* =========================
   UI UPDATE
   ========================= */
void ui_update(void)
{
    if (q8_active) return;

    static int last_s = -1;
    static uint32_t last_sensor_ui = 0;

    struct tm timeinfo;

    if (getLocalTime(&timeinfo))
    {
        int h = timeinfo.tm_hour;
        int m = timeinfo.tm_min;
        int s = timeinfo.tm_sec;

        update_theme(h);

        if (s != last_s)
        {
            last_s = s;

            lv_label_set_text_fmt(
                label_sec,
                "%02d",
                s
            );

            lv_label_set_text_fmt(
                label_time,
                "%02d:%02d",
                h,
                m
            );

            update_hands(h, m, s);

            const char *days[] = {
                "DOM","LUN","MAR",
                "MER","GIO","VEN","SAB"
            };

            lv_label_set_text(
                label_day,
                days[timeinfo.tm_wday]
            );

            lv_label_set_text_fmt(
                label_date,
                "%02d/%02d",
                timeinfo.tm_mday,
                timeinfo.tm_mon + 1
            );

            lv_label_set_text_fmt(
                label_year,
                "%04d",
                timeinfo.tm_year + 1900
            );
        }
    }

    updateSensor();

    if (millis() - last_sensor_ui > 3000)
    {
        last_sensor_ui = millis();

        float t = getTemperature();
        float h = getHumidity();

        char txt[64];

        snprintf(
            txt,
            sizeof(txt),
            "T: %.1f °C\nU: %.0f %%",
            t,
            h
        );

        lv_label_set_text(
            label_info,
            txt
        );
    }
}

/* =========================
   Q8 AP MODE
   ========================= */
void q8_enter_ap_mode(
    const char* ip,
    const char* pass
)
{
    if (q8_active) return;

    q8_active = true;

    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    lv_obj_t *label =
        lv_label_create(lv_scr_act());

    char buf[128];

    snprintf(
        buf,
        sizeof(buf),

        "AP MODE\n\n"
        "IP: %s\n"
        "PASS: %s",

        ip,
        pass
    );

    lv_label_set_text(label, buf);

    lv_obj_set_style_text_color(
        label,
        lv_color_white(),
        0
    );

    lv_obj_set_style_text_align(
        label,
        LV_TEXT_ALIGN_CENTER,
        0
    );

    lv_obj_center(label);
}