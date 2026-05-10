#include <WiFi.h>
#include <WebServer.h>

// ================= WiFi Credentials =================
const char* ssid = "Sensor";
const char* password = "12345678";

// ================= Web Server =================
WebServer server(80);

// ================= Pin Definitions =================
#define MQ2_PIN    34
#define DOOR_PIN   4
#define BUZZER_PIN 14
#define LDR_PIN    5

// ================= Variables =================
const int smokeThreshold = 1500;
unsigned long previousMillis = 0;

// ====================================================
// HTML Dashboard
// ====================================================
void handleRoot() {
  int smokeValue = analogRead(MQ2_PIN);
  int doorState  = digitalRead(DOOR_PIN);
  int ldrState   = digitalRead(LDR_PIN);

  String doorStatus  = (doorState == HIGH) ? "OPEN" : "CLOSED";
  String lightStatus = (ldrState == HIGH) ? "DARK" : "BRIGHT";

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
      <meta http-equiv="refresh" content="2">
      <title>ESP32 Home Automation</title>
      <style>
          body {
              font-family: Arial;
              text-align: center;
              background: #121212;
              color: white;
          }
          .card {
              background: #1e1e1e;
              margin: 20px auto;
              padding: 20px;
              width: 320px;
              border-radius: 15px;
              box-shadow: 0 0 15px rgba(255,255,255,0.1);
          }
      </style>
  </head>
  <body>
      <h1>🏠 Home Automation Dashboard</h1>
      <div class="card">
          <h2>🚪 Door: %DOOR%</h2>
          <h2>🔥 Smoke: %SMOKE%</h2>
          <h2>💡 Light: %LIGHT%</h2>
          %ALERT%
      </div>
  </body>
  </html>
  )rawliteral";

  html.replace("%DOOR%", doorStatus);
  html.replace("%SMOKE%", String(smokeValue));
  html.replace("%LIGHT%", lightStatus);

  if (smokeValue > smokeThreshold)
    html.replace("%ALERT%", "<h2 style='color:red;'>⚠ Smoke Detected!</h2>");
  else
    html.replace("%ALERT%", "<h2 style='color:lime;'>✅ Safe</h2>");

  server.send(200, "text/html", html);
}

// ====================================================
// Connect WiFi
// ====================================================
void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("🌐 IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi Connection Failed!");
    Serial.println("Check SSID, Password, or 2.4GHz Network.");
  }
}

// ====================================================
// Setup
// ====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(DOOR_PIN, INPUT_PULLUP);
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  connectWiFi();

  server.on("/", handleRoot);
  server.begin();

  Serial.println("🚀 Web Server Started!");
}

// ====================================================
// Loop
// ====================================================
void loop() {
  server.handleClient();

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();

    int smokeValue = analogRead(MQ2_PIN);
    int doorState  = digitalRead(DOOR_PIN);
    int ldrState   = digitalRead(LDR_PIN);

    // Serial Monitor Output
    Serial.println("\n==============================");
    Serial.print("🔥 Smoke Value : ");
    Serial.println(smokeValue);

    Serial.print("🚪 Door Status : ");
    Serial.println(doorState ? "OPEN" : "CLOSED");

    Serial.print("💡 Light Status: ");
    Serial.println(ldrState ? "DARK" : "BRIGHT");

    // Buzzer Logic
    if (smokeValue > smokeThreshold) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("⚠ ALERT: Smoke Detected!");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("✅ Environment Safe");
    }

    Serial.println("==============================");
  }
}
