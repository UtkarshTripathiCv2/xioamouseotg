#include <WiFi.h>
#include <WebServer.h>
#include "USB.h"
#include "USBHIDMouse.h"

USBHIDMouse Mouse;
WebServer server(80);

const char* ssid = "XIAO-Mouse";
const char* pass = "12345678";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <style>
    body { margin:0; background:#111; color:#fff; font-family:sans-serif; display:flex; flex-direction:column; height:100vh; overflow:hidden; }
    #pad { flex:1; background:#1e1e1e; margin:12px; border-radius:16px; display:flex; align-items:center; justify-content:center; color:#666; touch-action:none; border:1px solid #333; }
    .btns { display:flex; height:90px; gap:12px; margin:0 12px 12px; }
    button { flex:1; background:#2a2a2a; color:#fff; border:none; border-radius:12px; font-size:18px; }
    button:active { background:#0078ff; }
  </style>
</head>
<body>
  <div id="pad">Drag finger here to move cursor</div>
  <div class="btns">
    <button onclick="send('/c?b=l')">Left Click</button>
    <button onclick="send('/c?b=r')">Right Click</button>
  </div>
  <script>
    let lx = null, ly = null, busy = false;
    const pad = document.getElementById('pad');
    function send(url) { fetch(url).catch(()=>{}); }

    pad.addEventListener('touchstart', e => {
      lx = e.touches[0].clientX;
      ly = e.touches[0].clientY;
    });

    pad.addEventListener('touchmove', e => {
      e.preventDefault();
      if (busy || lx === null) return;
      let cx = e.touches[0].clientX, cy = e.touches[0].clientY;
      let dx = Math.round((cx - lx) * 1.8);
      let dy = Math.round((cy - ly) * 1.8);
      lx = cx; ly = cy;
      if (dx !== 0 || dy !== 0) {
        busy = true;
        fetch(`/m?x=${dx}&y=${dy}`).finally(() => busy = false);
      }
    });
  </script>
</body>
</html>
)rawliteral";

void setup() {
  pinMode(0, INPUT_PULLUP); // Onboard BOOT button = Left Click backup
  Mouse.begin();
  USB.begin();

  WiFi.softAP(ssid, pass);
  server.on("/", []() { server.send_P(200, "text/html", index_html); });
  server.on("/m", []() {
    int dx = server.arg("x").toInt();
    int dy = server.arg("y").toInt();
    Mouse.move(dx, dy);
    server.send(200, "text/plain", "ok");
  });
  server.on("/c", []() {
    if (server.arg("b") == "r") Mouse.click(MOUSE_RIGHT);
    else Mouse.click(MOUSE_LEFT);
    server.send(200, "text/plain", "ok");
  });
  server.begin();
}

void loop() {
  server.handleClient();
  // Physical BOOT button also acts as a Left Click
  if (digitalRead(0) == LOW) {
    Mouse.click(MOUSE_LEFT);
    delay(250);
  }
}
