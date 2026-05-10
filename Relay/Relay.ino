#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <AceButton.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>

using namespace ace_button;

// ======================================================
// Function Declarations
// ======================================================
void handleRoot();
void handleUpdate();
void toggleRelay(uint8_t pin, int addr, bool &state);
void connectWiFi();

// ======================================================
// WiFi Credentials
// ======================================================
const char* ssid = "Sensor";
const char* password = "12345678";

ESP8266WebServer server(80);

// ======================================================
// Pin Definitions
// ======================================================
#define EEPROM_SIZE 10

#define RelayPin1 D5
#define RelayPin2 D6
#define RelayPin3 D7
#define RelayPin4 D0

#define SwitchPin1 D1
#define SwitchPin2 D2
#define SwitchPin3 D3
#define SwitchPin4 D4

// ======================================================
// Variables
// ======================================================
bool state1, state2, state3, state4;

int smokeValue = 0;
int doorState = 0;
int ldrState = 0;

ButtonConfig config1, config2, config3, config4;
AceButton btn1(&config1), btn2(&config2), btn3(&config3), btn4(&config4);

// ======================================================
// EEPROM Functions
// ======================================================
void writeEEPROM(int addr, bool state) {
  EEPROM.write(addr, state);
  EEPROM.commit();
}

bool readEEPROM(int addr) {
  return EEPROM.read(addr);
}

// ======================================================
// Relay Functions
// ======================================================
void setRelay(uint8_t pin, int addr, bool state) {
  digitalWrite(pin, !state);   // Active LOW relay
  writeEEPROM(addr, state);
}

void toggleRelay(uint8_t pin, int addr, bool &state) {
  state = !state;
  setRelay(pin, addr, state);
}

// ======================================================
// Button Handler
// ======================================================
void handleButton(AceButton* button, uint8_t eventType,
                  uint8_t buttonState, uint8_t relayPin,
                  int addr, bool &state) {
  if (eventType == AceButton::kEventReleased) {
    toggleRelay(relayPin, addr, state);
  }
}

// ======================================================
// WiFi Connection
// ======================================================
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to WiFi");

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected Successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("WiFi Connection Failed!");
    Serial.println("Check SSID, Password, or 2.4GHz Network.");
  }
}

// ======================================================
// Dashboard
// ======================================================
/*
Replace ONLY your handleRoot() function with this one.
It creates:
1. Home Appliances Page (ESP1 - Relays)
2. Real-Time Monitoring Page (ESP2 - Sensors)
3. Sidebar Navigation
4. Smooth Mobile Responsive UI
*/

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Smart Home Automation</title>

<style>
*{
  margin:0;
  padding:0;
  box-sizing:border-box;
  font-family:Segoe UI,sans-serif;
}

body{
  background:linear-gradient(135deg,#d9f3f4,#c8e6e7);
  overflow:hidden;
}

.container{
  display:flex;
  height:100vh;
}

/* Sidebar */
.sidebar{
  width:270px;
  background:#050505;
  color:white;
  padding:30px 20px;
}

.logo{
  font-size:35px;
  font-weight:bold;
  margin-bottom:50px;
}

.menu-btn{
  display:block;
  width:100%;
  padding:18px;
  margin:15px 0;
  border:none;
  border-radius:15px;
  background:#111;
  color:white;
  font-size:20px;
  cursor:pointer;
  transition:0.3s;
}

.menu-btn:hover,
.menu-btn.active{
  background:#d8d76f;
  color:black;
}

/* Main */
.main{
  flex:1;
  padding:35px;
  overflow-y:auto;
}
/* Add this CSS */

.logo-section {
    text-align: center;
    padding: 20px 10px;
    margin-bottom: 30px;
}

.nit-logo {
    width: 130px;
    height: 130px;
    object-fit: contain;
    border-radius: 50%;
    background: white;
    padding: 8px;
    margin-bottom: 15px;
    box-shadow: 0 0 20px rgba(255,255,255,0.2);
}

.logo-section h1 {
    color: white;
    font-size: 38px;
    font-weight: bold;
    margin: 0;
    letter-spacing: 2px;
    text-transform: uppercase;
}
.page{
  display:none;
}

.page.active{
  display:block;
}

h1{
  font-size:50px;
  margin-bottom:30px;
  color:#111;
}

/* Cards */
.grid{
  display:grid;
  grid-template-columns:repeat(auto-fit,minmax(280px,1fr));
  gap:25px;
}

.card{
  background:#232424;
  color:white;
  padding:30px;
  border-radius:30px;
  text-align:center;
  box-shadow:0 20px 40px rgba(0,0,0,0.25);
}

.card h2{
  margin-bottom:20px;
  color:#f7f505;
}

.card1{
  background:#232424;
  color:white;
  padding:30px;
  border-radius:30px;
  text-align:center;
  box-shadow:0 20px 40px rgba(0,0,0,0.25);
}

.card1 h2{
  margin-bottom:20px;
  color:#f7f505;
}

.card2{
  background:#232424;
  color:white;
  padding:30px;
  border-radius:30px;
  text-align:center;
  box-shadow:0 20px 40px rgba(0,0,0,0.25);
}

.card2 h2{
  margin-bottom:20px;
  color:#2c20d6;
}

.card3{
  background:#232424;
  color:white;
  padding:30px;
  border-radius:30px;
  text-align:center;
  box-shadow:0 20px 40px rgba(0,0,0,0.25);
}

.card3 h2{
  margin-bottom:20px;
  color:#d62920;
}

.card4{
  background:#232424;
  color:white;
  padding:30px;
  border-radius:30px;
  text-align:center;
  box-shadow:0 20px 40px rgba(0,0,0,0.25);
}

.card4 h2{
  margin-bottom:20px;
  color:#3fe815;
}

.value{
  font-size:45px;
  font-weight:bold;
}

/* Relay Buttons */
.relay-btn{
  width:100%;
  padding:20px;
  border:none;
  border-radius:20px;
  font-size:26px;
  cursor:pointer;
  margin-top:20px;
  transition:0.3s;
}

.on{
  background:#d8d76f;
  color:black;
}

.off{
  background:#ff3b30;
  color:white;
}

.relay-btn:hover{
  transform:scale(1.05);
}


.smoke-alert{
  position:fixed;
  top:20px;
  right:20px;
  background:#ff3b30;
  color:#fff;
  padding:18px 28px;
  border-radius:18px;
  font-size:22px;
  font-weight:bold;
  box-shadow:0 10px 30px rgba(0,0,0,0.35);
  display:none;
  z-index:9999;
  animation:blink 1s infinite;
}

@keyframes blink{
  0%,100%{opacity:1;}
  50%{opacity:0.4;}
}


</style>
</head>
<body>

<div class="container">

  <div class="sidebar">

    <div class="logo-section">
        <h1>NIT GOA</h1>
    </div>

    <button class="menu-btn active" onclick="showPage('appliances',this)">
      Home Appliances
    </button>

    <button class="menu-btn" onclick="showPage('sensors',this)">
      Real-Time Data
    </button>
  </div>

  <div class="main">

    <!-- HOME APPLIANCES PAGE -->
    <div id="appliances" class="page active">
      <h1>Home Appliances</h1>

      <div class="grid">
        <div class="card1">
          <h2>Appliance 1</h2>
          <button class="relay-btn %R1%" onclick="toggleRelay('/r1')">
            %T1%
          </button>
        </div>

        <div class="card2">
          <h2>Appliance 2</h2>
          <button class="relay-btn %R2%" onclick="toggleRelay('/r2')">
            %T2%
          </button>
        </div>

        <div class="card3">
          <h2>Appliance 3</h2>
          <button class="relay-btn %R3%" onclick="toggleRelay('/r3')">
            %T3%
          </button>
        </div>

        <div class="card4">
          <h2>Appliance 4</h2>
          <button class="relay-btn %R4%" onclick="toggleRelay('/r4')">
            %T4%
          </button>
        </div>
      </div>
    </div>

    <!-- SENSOR PAGE -->
    <div id="sensors" class="page">
      <h1>Real-Time Monitoring</h1>

      <div class="grid">
        <div class="card">
          <h2>Smoke Level</h2>
          <div class="value" id="smoke">0</div>
        </div>

        <div class="card">
          <h2>Door Status</h2>
          <div class="value" id="door">---</div>
        </div>

        <div class="card">
          <h2>Light Status</h2>
          <div class="value" id="light">---</div>
        </div>
      </div>
    </div>

  </div>
</div>

<script>
function showPage(page,btn){
  document.querySelectorAll('.page').forEach(p=>p.classList.remove('active'));
  document.querySelectorAll('.menu-btn').forEach(b=>b.classList.remove('active'));

  document.getElementById(page).classList.add('active');
  btn.classList.add('active');
}

function toggleRelay(url){
  fetch(url)
  .then(()=>location.reload());
}

// Replace your existing updateData() JavaScript function with this one

function updateData() {
  fetch('/data')
    .then(response => response.json())
    .then(data => {
      // Update Smoke
      document.getElementById('smoke').textContent = data.smoke;

      // Update Door
      document.getElementById('door').textContent =
        (data.door === "OPEN" || data.door == 1) ? "OPEN" : "CLOSED";

      // Update Light
      document.getElementById('light').textContent =
        (data.light === "DARK" || data.light == 1) ? "DARK" : "BRIGHT";

      // Smoke Alert
      const SMOKE_THRESHOLD = 1500;   // Same as ESP32 threshold

      if (parseInt(data.smoke) > SMOKE_THRESHOLD) {
          smokeAlert.style.display = 'block';
      } else {
          smokeAlert.style.display = 'none';
      }
    })
    .catch(error => console.log("Data fetch error:", error));
}

// Refresh every second
setInterval(updateData, 1000);
updateData();
function toggleRelay(url){
  fetch(url).then(() => location.reload());
}

setInterval(updateData, 2000);
updateData();
</script>

</body>
<div id="smokeAlert" class="smoke-alert">
  ⚠ Smoke Detected!
</div>
</html>
)rawliteral";

  html.replace("%R1%", state1 ? "on" : "off");
  html.replace("%R2%", state2 ? "on" : "off");
  html.replace("%R3%", state3 ? "on" : "off");
  html.replace("%R4%", state4 ? "on" : "off");

  html.replace("%T1%", state1 ? "ON" : "OFF");
  html.replace("%T2%", state2 ? "ON" : "OFF");
  html.replace("%T3%", state3 ? "ON" : "OFF");
  html.replace("%T4%", state4 ? "ON" : "OFF");

  server.send(200, "text/html", html);
}// Add this function anywhere below handleRoot()

void handleUpdate() {
  if (server.hasArg("smoke")) {
    smokeValue = server.arg("smoke").toInt();
  }

  if (server.hasArg("door")) {
    doorState = server.arg("door").toInt();
  }

  if (server.hasArg("light")) {
    ldrState = server.arg("light").toInt();
  }

  server.send(200, "text/plain", "OK");
}

// ======================================================
// Setup
// ======================================================
void setup() {
  server.serveStatic("/NIT_Goa_Logo.png", LittleFS, "/NIT_Goa_Logo.png");
  Serial.begin(115200);
  if (!LittleFS.begin()) {
  Serial.println("LittleFS Mount Failed");
  return;
}
  delay(2000);

  EEPROM.begin(EEPROM_SIZE);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  state1 = readEEPROM(0);
  state2 = readEEPROM(1);
  state3 = readEEPROM(2);
  state4 = readEEPROM(3);

  setRelay(RelayPin1, 0, state1);
  setRelay(RelayPin2, 1, state2);
  setRelay(RelayPin3, 2, state3);
  setRelay(RelayPin4, 3, state4);

  connectWiFi();

  // Web Routes
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);

  server.on("/data", []() {
    String json = "{";
    json += "\"smoke\":" + String(smokeValue) + ",";
    json += "\"door\":\"" + String(doorState ? "OPEN" : "CLOSED") + "\",";
    json += "\"light\":\"" + String(ldrState ? "DARK" : "BRIGHT") + "\"";
    json += "}";
    server.send(50, "application/json", json);
  });

  server.on("/r1", []() {
    toggleRelay(RelayPin1, 0, state1);
    server.send(200, "text/plain", "OK");
  });

  server.on("/r2", []() {
    toggleRelay(RelayPin2, 1, state2);
    server.send(200, "text/plain", "OK");
  });

  server.on("/r3", []() {
    toggleRelay(RelayPin3, 2, state3);
    server.send(200, "text/plain", "OK");
  });

  server.on("/r4", []() {
    toggleRelay(RelayPin4, 3, state4);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("Web Server Started");

  // Button Configuration
  config1.setEventHandler([](AceButton*, uint8_t e, uint8_t s) {
    handleButton(nullptr, e, s, RelayPin1, 0, state1);
  });

  config2.setEventHandler([](AceButton*, uint8_t e, uint8_t s) {
    handleButton(nullptr, e, s, RelayPin2, 1, state2);
  });

  config3.setEventHandler([](AceButton*, uint8_t e, uint8_t s) {
    handleButton(nullptr, e, s, RelayPin3, 2, state3);
  });

  config4.setEventHandler([](AceButton*, uint8_t e, uint8_t s) {
    handleButton(nullptr, e, s, RelayPin4, 3, state4);
  });

  btn1.init(SwitchPin1);
  btn2.init(SwitchPin2);
  btn3.init(SwitchPin3);
  btn4.init(SwitchPin4);
}

// ======================================================
// Loop
// ======================================================
void loop() {
  btn1.check();
  btn2.check();
  btn3.check();
  btn4.check();

  server.handleClient();
}
