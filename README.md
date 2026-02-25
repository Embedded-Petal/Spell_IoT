#  Spell_IoT Arduino Library

Spell_IoT is a universal IoT HTTP communication library for ESP32 and ESP8266.

## Features
- ESP32 & ESP8266 support
- Virtual pin read & write
- RGB HEX color parsing
- Common polling interval
- Automatic board detection

## Supported Boards
- ESP32
- ESP8266

## Installation
1. Open Arduino IDE
2. Go to Library Manager
3. Search for "Spell_IoT","IoTCloud" 
4. Install

## Example
```
#include <Spell_IoT.h>
#define WIFI_SSID     "SSID"
#define WIFI_PASSWORD "Password"
#define DEVICE_TOKEN  "Auth token"

#define LED1 13
int LEDState1;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  Spelliot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Spelliot.registerPin("V0", [](String v) {
    LEDState1 = v.toInt();
  });
}

void loop() {
  Spelliot.loop();
  if (LEDState1 == 1 ) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
}  

```
## Author
- [Karthickraja Murugan](https://github.com/karthickrajathedeveloper)
