#include "camera_fw.h"
#include "src/config.h"

#include <WiFi.h>
#include <esp_camera.h>
#include <WebServer.h>

/* =====================================
   STREAM SETTINGS
===================================== */
#define STREAM_FPS        25
#define STREAM_DELAY_MS   1

/* =====================================
   STATIC STATE
===================================== */
static bool cam_active = false;
static bool cam_initialized = false;

static WebServer camServer(
    STREAM_PORT_LOCAL
);

/* =====================================
   CAMERA CONFIG
===================================== */
static camera_config_t cam_cfg = {

    .pin_pwdn  = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk  = XCLK_GPIO_NUM,

    .pin_sccb_sda = SIOD_GPIO_NUM,
    .pin_sccb_scl = SIOC_GPIO_NUM,

    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,

    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href  = HREF_GPIO_NUM,
    .pin_pclk  = PCLK_GPIO_NUM,

    .xclk_freq_hz = 20000000,

    .ledc_timer   = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,

    .frame_size   = FRAMESIZE_QVGA,
    .jpeg_quality = 12,

    .fb_count     = 2
};

/* =====================================
   STREAM HANDLER
===================================== */
static void handle_stream()
{
    if (!cam_active)
    {
        camServer.send(
            403,
            "text/plain",
            "Camera not active"
        );

        return;
    }

    WiFiClient client = camServer.client();

    client.println("HTTP/1.1 200 OK");

    client.println(
        "Content-Type: multipart/x-mixed-replace; boundary=frame"
    );

    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
    client.println();

    while (client.connected() && cam_active)
    {
        camera_fb_t *fb = esp_camera_fb_get();

        if (!fb)
        {
            delay(10);
            continue;
        }

        client.print("--frame\r\n");

        client.print(
            "Content-Type: image/jpeg\r\n"
        );

        client.print("Content-Length: ");

        client.print(fb->len);

        client.print("\r\n\r\n");

        client.write(fb->buf, fb->len);

        client.print("\r\n");

        esp_camera_fb_return(fb);

        client.flush();

        delay(10);

        yield();
    }
}
/* =====================================
   ROOT PAGE
===================================== */
static void handle_root()
{
    String info;

    info += "ESP CAMERA STREAM ONLINE\n";
    info += "DEVICE: ";
    info += DEVICE_ID;
    info += "\n";

    info += "USER: ";
    info += USER_FIN;
    info += "\n";

    info += "STREAM PORT: ";
    info += String(STREAM_PORT_LOCAL);
    info += "\n";

    info += "STATUS: ";

    if (cam_active)
    {
        info += "ACTIVE";
    }
    else
    {
        info += "STOPPED";
    }

    info += "\n";

    camServer.send(
        200,
        "text/plain",
        info
    );
}

/* =====================================
   STOP HANDLER
===================================== */
static void handle_stop()
{
    cam_active = false;

    camServer.send(
        200,
        "text/plain",
        "Camera stopped"
    );

    delay(300);

    camServer.stop();
}

/* =====================================
   STATUS HANDLER
===================================== */
static void handle_status()
{
    String status;

    status += "{";
    status += "\"device\":\"";
    status += DEVICE_ID;
    status += "\",";
    
    status += "\"user\":\"";
    status += USER_FIN;
    status += "\",";
    
    status += "\"active\":";

    if (cam_active)
    {
        status += "true";
    }
    else
    {
        status += "false";
    }

    status += "}";

    camServer.send(
        200,
        "application/json",
        status
    );
}

/* =====================================
   PUBLIC API
===================================== */

bool cameraStart(void)
{
    if (cam_active)
    {
        return true;
    }

    if (!cam_initialized)
    {
        if (esp_camera_init(&cam_cfg) != ESP_OK)
        {
            Serial.println("[CAM] init failed");
            return false;
        }

        cam_initialized = true;

        Serial.println("[CAM] init OK");
    }

    cam_active = true;

    camServer.on(
        "/",
        HTTP_GET,
        handle_root
    );

    camServer.on(
        "/stream",
        HTTP_GET,
        handle_stream
    );

    camServer.on(
        "/stop",
        HTTP_GET,
        handle_stop
    );

    camServer.on(
        "/status",
        HTTP_GET,
        handle_status
    );

    camServer.begin();

    Serial.print(
        "[CAM] server started on "
    );

    Serial.println(
        STREAM_PORT_LOCAL
    );

    return true;
}

void cameraStop(void)
{
    cam_active = false;

    camServer.stop();

    Serial.println("[CAM] stopped");
}

void cameraHandle(void)
{
    if (!cam_active)
    {
        return;
    }

    camServer.handleClient();
}

bool cameraIsActive(void)
{
    return cam_active;
}