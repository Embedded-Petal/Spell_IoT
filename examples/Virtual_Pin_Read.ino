/*************************************************************

  You’ll need:
   - Spell IoT app (download from App Store or Google Play)
   - ESP32/ESP8266 board
   - Decide how to connect to SPELLIOT
     (Ethernet, Wi-Fi )
  Code Developed By : M.Karthickraja
  Date : 25-02-2026
  *************************************************************/

/* Fill-in information from SPELLIoT Device Info here */

#include <Spell_IoT.h>

#define WIFI_SSID     "**************"
#define WIFI_PASSWORD  "**************"
#define DEVICE_TOKEN  "*******************"

#define LED1 13
int LEDState1;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Spell_iot.registerPin("V0", [](String v) {
    LEDState1 = v.toInt();
  });
}

void loop() {
  Spell_iot.loop();
  if (LEDState1 == 1 ) 
    digitalWrite(LED1, HIGH);
  else
    digitalWrite(LED1, LOW);
}  
