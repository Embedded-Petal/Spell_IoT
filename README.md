# Spell_IoT Arduino Library

Spell_IoT is a universal IoT communication library for ESP32 and ESP8266. It supports both WebSockets (default) and MQTT protocols for seamless cloud connectivity.

## Features
- **Dual Protocols:** Built-in support for both WebSockets and MQTT.
- **Background Processing:** Network handling runs entirely in background tasks (FreeRTOS for ESP32, Ticker for ESP8266). No need to call `loop()` manually!
- **Universal JSON Support:** Fully compatible with both ArduinoJson Version 6 and Version 7.
- **Easy Virtual Pins:** Simple virtual pin read & write mechanism.
- **RGB Parsing:** Built-in RGB HEX color parsing.
- **OTA Updates:** Support for SpellIoT AIR firmware updates over the air.

## Supported Boards
- ESP32
- ESP8266

## Installation
1. Open Arduino IDE
2. Go to Library Manager
3. Search for "Spell_IoT"
4. Install

## Examples

We provide 28+ ready-to-use examples for various sensors in two formats:
- **`program with cpp`**: Uses the standard WebSockets backend.
- **`program with mqtt`**: Uses the new lightweight MQTT backend.

### 1. Basic Example (WebSockets)
```cpp
#include <Spell_IoT.h>

#define WIFI_SSID     "SSID"
#define WIFI_PASSWORD "Password"
#define DEVICE_TOKEN  "Auth token"

#define LED1 13

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  
  Spell_iot.registerPin("V0", [](String v) {
    digitalWrite(LED1, v.toInt() ? HIGH : LOW);
  });
}

void loop() {
  // Your code here. No need to call Spell_iot.loop()!
}
```

### 2. MQTT Example
To use MQTT instead of WebSockets, simply define `SPELLIOT_MQTT` before including the library.

```cpp
#define SPELLIOT_MQTT
#include <Spell_IoT.h>

#define WIFI_SSID     "SSID"
#define WIFI_PASSWORD "Password"
#define DEVICE_TOKEN  "Auth token"

#define LED1 13

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  
  Spell_iot.registerPin("V0", [](String v) {
    digitalWrite(LED1, v.toInt() ? HIGH : LOW);
  });
}

void loop() {
  // Your code here. No need to call Spell_iot.loop()!
}
```

## Author
- [Karthickraja Murugan](https://github.com/karthickrajathedeveloper)
