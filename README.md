# IoT-Based Home Automation System

A cost-effective smart home automation system built using the **ESP8266 microcontroller**, enabling real-time remote monitoring and control of home appliances over Wi-Fi. Developed as a B.Tech project at **NIT Goa** (April 2026).

---

## Features

- **Smoke & Gas Detection** — MQ-2 sensor monitors air for smoke, LPG, and butane leaks; sends instant alerts to the user's mobile device via the internet
- **Remote Appliance Control** — 4-channel relay module allows toggling of multiple home appliances simultaneously through a Wi-Fi interface
- **Magnetic Door Security** — Magnetic contact sensor detects unauthorized door openings and triggers real-time server notifications
- **Ambient Light Monitoring** — LDR (Light Dependent Resistor) tracks ambient light levels for automated lighting control
- **2.4GHz Wi-Fi Communication** — All sensor data and control commands transmitted wirelessly using the ESP8266's built-in Wi-Fi module
- **Real-time Remote Access** — Monitor and control all modules from anywhere in the world via internet connectivity

---

## System Architecture

```
Sensors / Actuators          ESP8266 Microcontroller          Cloud / User
─────────────────            ───────────────────────          ────────────
MQ-2 Gas Sensor      ──►
Magnetic Door Sensor ──►     Wi-Fi (2.4GHz)          ──►     Cloud Server
LDR Sensor           ──►     Signal Processing        ◄──     Mobile Alerts
4-Channel Relay      ◄──     Command Handling                 Remote Control
```

---

## Hardware Components

| Component | Description | Cost (₹) |
|---|---|---|
| ESP8266 Microcontroller | Wi-Fi enabled MCU for data processing and connectivity | 275 |
| 4-Channel Relay Module | Controls multiple home appliances (ON/OFF) | 350 |
| MQ-2 Gas Sensor | Detects smoke, LPG, and butane gas leaks | 150 |
| Magnetic Door Lock Sensor | Detects unauthorized door openings | 150 |
| LM393 Comparator Module | Signal conditioning for sensor outputs (includes LDR) | 150 |
| Resistors, Breadboard, Jumper Wires | Pre-existing lab resources | — |
| **Total** | | **~₹1,350** |

---

## Modules

### 1. Smoke & Gas Detection
Uses the MQ-2 sensor to continuously monitor air quality. On detecting high concentrations of smoke or gas, the ESP8266 immediately transmits an alert to the user's mobile device over Wi-Fi and can receive remote reset commands.

### 2. Home Appliances Control
A 4-channel relay module connected to the ESP8266 enables wireless toggling of electrical loads — fans, lights, and other appliances — based on user commands sent via a mobile/web interface.

### 3. Magnetic Door Security
A magnetic contact sensor monitors door state. When the magnetic contact is broken (door opened), the ESP8266 sends an intrusion alert to the cloud server, notifying the homeowner in real time.

### 4. Ambient Light Monitoring (LDR)
An LDR connected through the LM393 comparator module detects ambient light levels, enabling automated lighting decisions based on day/night conditions.

---

## Setup & Installation

### Prerequisites
- Arduino IDE (v1.8+ or v2.x)
- ESP8266 board package installed in Arduino IDE
- Wi-Fi network credentials

### Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/Hardik-jain20/INTERNET-OF-THINGS-IOT-BASED-HOME-AUTOMATION-SYSTEM.git
   cd INTERNET-OF-THINGS-IOT-BASED-HOME-AUTOMATION-SYSTEM
   ```

2. **Open in Arduino IDE**
   - Open the `.ino` file from the `Sensor/` or `Relay/` folder depending on the module

3. **Configure Wi-Fi credentials**
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

4. **Select board and port**
   - Board: `NodeMCU 1.0 (ESP-12E Module)`
   - Port: Your COM port

5. **Upload the code** to the ESP8266 and open Serial Monitor at 115200 baud to verify connection

---

## Project Structure

```
├── Relay/          # Code for relay-based appliance control
├── Sensor/         # Code for sensor modules (MQ-2, door sensor, LDR)
└── README.md
```

---

## Future Scope

- Integration with smart city infrastructure and medical monitoring systems
- Offline/autonomous operation without active internet connectivity
- Expansion to support thousands of IoT devices simultaneously
- Enhanced data security and encryption for all transmissions
- Voice assistant integration (Alexa / Google Home)

---

## Team

Developed by B.Tech ECE students at **National Institute of Technology Goa** under the supervision of **Dr. Mallikarjun Erramshetty** (Assistant Professor, Dept. of ECE).

| Name | Roll No. |
|---|---|
| Darshan Lamani | 24ECE1008 |
| Hardik Jain | 24ECE1013 |
| Anup Jha | 24ECE1017 |
| Krrish Kunkolikar | 24ECE1023 |
| Paras Morajkar | 24ECE1032 |

---

## References

- S. Vicky — *IoT: The Best Alternative for Home Automation*
- Yoti Rani, Prerna Chauhan, Ritika Tripathi — *IoT: The Future Technology in Wireless Automation*, International Journal of Applied Engineering
- ResearchGate — *Survey on IoT Home Security and its Applications*
- Svilen Dimitrov & Harald Haas — *Principles of Smart Communications*

---

## License

This project was developed for academic purposes at NIT Goa. Feel free to use and build upon it.
