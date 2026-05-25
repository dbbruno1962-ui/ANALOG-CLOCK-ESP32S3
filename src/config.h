/* =====================================================
 * config.h
 * ===================================================== */

#pragma once

/* =====================================================
 * PINOUT TFT
 * ===================================================== */

#define LCD_CS   14
#define LCD_RST  21
#define LCD_DC   47
#define LCD_BL   0

#define SPI_MOSI 45
#define SPI_MISO 46
#define SPI_SCLK 3

/* =====================================================
 * DHT11 SENSOR
 * ===================================================== */

#define DHT_PIN   2
#define DHT_TYPE  DHT11

/* =====================================================
 * USER PROFILE
 * ===================================================== */
#define SERVER_HOST "www.giocando.net"
#define USER_FIN_EMILIA
//#define USER_FIN_ANTONELLA
//#define USER_FIN_TOMMASO
//#define USER_FIN_BRUNO

/* =====================================================
 * USER CONFIG
 * ===================================================== */

#ifdef USER_FIN_EMILIA

    #define USER_FIN            "emilia"

    #define DEVICE_ID           "esp_clock_emilia"

    #define STREAM_PORT_LOCAL   81
    #define STREAM_PORT_PUBLIC  25402

#endif


#ifdef USER_FIN_ANTONELLA

    #define USER_FIN            "antonella"

    #define DEVICE_ID           "esp_clock_antonella"

    #define STREAM_PORT_LOCAL   82
    #define STREAM_PORT_PUBLIC  25401

#endif


#ifdef USER_FIN_TOMMASO

    #define USER_FIN            "tommaso"

    #define DEVICE_ID           "esp_clock_tommaso"

    #define STREAM_PORT_LOCAL   83
    #define STREAM_PORT_PUBLIC  25403

#endif


#ifdef USER_FIN_BRUNO

    #define USER_FIN            "bruno"

    #define DEVICE_ID           "esp_clock_bruno"

    #define STREAM_PORT_LOCAL   84
    #define STREAM_PORT_PUBLIC  25404

#endif

// ================= CAMERA PIN DEDICATED =================
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5
#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

// ================= CAMERA SYNC =================
#define PCLK_GPIO_NUM   13   // <-- pin PCLK reale della tua S3 CAM
#define VSYNC_GPIO_NUM  6    // <-- pin VSYNC
#define HREF_GPIO_NUM   7   // <-- pin HREF

// ================ LED ======================

#define W_28_12B_NUM 48
#define TX_LED_NUM 43
#define RX_LED_NUM 44

/* ===================================================== */
/* WIFI / NTP                                            *
/* ===================================================== */
#define WIFI_SSID     ""
#define WIFI_PASSWORD ""
#define WIFI_TIMEOUT_MS     15000
#define WIFI_AP_SSID        "Clock-AP"
#define NTP_SERVER          "pool.ntp.org"
#define GMT_OFFSET_SEC      3600
#define DAYLIGHT_OFFSET_SEC 3600
#define poll_name "pooleer"
#define poll_psw  "Alfanum563098"


/* ========= DISPLAY / LAYOUT POLICY =================== */

// Q0 (tempo) area dominante
#define Q0_H_PCT 0.47f
/* =========== Q0 – OPTICAL ADJUSTMENTS ================ */
#define Q0_MINUTE_X_OFFSET   (-4)
// padding orizzontale standard
#define PAD_X 10
// influenza tipografica (NON layout geometrico)
#define FONT_INFLUENCE_Q1 0.30f   // data / eventi
#define FONT_INFLUENCE_Q2 0.30f   // sensori

/* ============ COLOR PALETTE – ARLECCHINO (DEFINITIVA)============= */
#define WHITE     0xFFFF
#define BLACK     0x0000
#define RED       0xF800
#define GREEN     0x07E0
#define BLUE      0x001F
#define YELLOW    0xFFE0
#define DARKGREY  0x7BEF

/* =======================LOCALIZED DATE (ITALIANO) == */
static const char *wdays_it[] = {
  "DOMENICA  ", "LUNEDI'  ", "MARTEDI'  ", "MERCOLEDI'  ", "GIOVEDI'  ", "VENERDI'  ", "SABATO  "
};
static const char *wdays_short_it[] = {
  "DOM", "LUN", "MAR", "MER", "GIO", "VEN", "SAB"
};

static const char *months_it[] = {
  "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno",
  "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"
};
