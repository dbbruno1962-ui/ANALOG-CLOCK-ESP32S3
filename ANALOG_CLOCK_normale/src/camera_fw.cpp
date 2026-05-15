#include "camera_fw.h"
#include "src/config.h"

#include <WiFi.h>
#include <esp_camera.h>
#include <WebServer.h>

/* =====================================
   STREAM SETTINGS
===================================== */
#define STREAM_FPS        15
#define STREAM_DELAY_MS   (1000 / STREAM_FPS)

/* =====================================
   STATIC STATE
===================================== */
static bool cam_active = false;
static bool cam_initialized = false;
static WebServer camServer(81);

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
static void handle_stream() {

  if (!cam_active) {
    camServer.send(403, "text/plain", "Camera not active");
    return;
  }

  WiFiClient client = camServer.client();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println();

  while (client.connected() && cam_active) {

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) continue;

    client.println("--frame");
    client.println("Content-Type: image/jpeg");
    client.print("Content-Length: ");
    client.println(fb->len);
    client.println();

    client.write(fb->buf, fb->len);
    client.println();

    esp_camera_fb_return(fb);

    delay(STREAM_DELAY_MS);
  }
}

/* =====================================
   ROOT PAGE
===================================== */
static void handle_root() {

  const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{
  margin:0;
  background:#000;
  display:flex;
  flex-direction:column;
  align-items:center;
  justify-content:center;
  height:100vh;
  font-family:sans-serif;
}

.frame{
  width:320px;
  height:240px;
  border:2px solid #444;
  background:#111;
}

img{
  width:100%;
  height:100%;
  display:block;
}

.closebtn{
  margin-top:20px;
  padding:12px 20px;
  font-size:18px;
  border:none;
  border-radius:8px;
  background:#e74c3c;
  color:#fff;
}
</style>
</head>

<body>

<div class="frame">
  <img src="/stream">
</div>

<button class="closebtn" onclick="window.close()">
  CHIUDI
</button>

</body>
</html>
)rawliteral";

  camServer.send(200, "text/html", html);
}


/* =====================================
   STOP HANDLER
===================================== */
static void handle_stop() {

  cam_active = false;

  camServer.send(200, "text/html",
    "<html><body style='background:#000;color:#fff;text-align:center;'>"
    "<h2>Camera Stopped</h2>"
    "</body></html>");

  delay(300);
  camServer.stop();
}

/* =====================================
   PUBLIC API
===================================== */

bool cameraStart(void)
{
  if (cam_active) return true;

  if (!cam_initialized) {
    if (esp_camera_init(&cam_cfg) != ESP_OK) {
      Serial.println("[CAM] init failed");
      return false;
    }
    cam_initialized = true;
    Serial.println("[CAM] init OK");
  }

  cam_active = true;

  camServer.on("/", HTTP_GET, handle_root);
  camServer.on("/stream", HTTP_GET, handle_stream);
  camServer.on("/stop", HTTP_GET, handle_stop);

  camServer.begin();

  Serial.println("[CAM] server started on 81");

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
  if (!cam_active) return;
  camServer.handleClient();
}

bool cameraIsActive(void)
{
  return cam_active;
}
