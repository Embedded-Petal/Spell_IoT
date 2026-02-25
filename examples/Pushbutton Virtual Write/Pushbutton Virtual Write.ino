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

#define WIFI_SSID "************"
#define WIFI_PASSWORD "*************"
#define DEVICE_TOKEN "**********************"

#define Button4 36
#define Button3 39
#define Button2 34
#define Button1 35

void setup() {
  Serial.begin(115200);
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Spell_iot.loop();
  if (digitalRead(Button1) == 0)
    Spell_iot.write("V0", 1);
  else
    Spell_iot.write("V0", 0);

  if (digitalRead(Button2) == 0)
    Spell_iot.write("V1", 1);
  else
    Spell_iot.write("V1", 0);

  if (digitalRead(Button3) == 0)
    Spell_iot.write("V2", 1);
  else
    Spell_iot.write("V2", 0);

  if (digitalRead(Button4) == 0)
    Spell_iot.write("V3", 1);
  else
    Spell_iot.write("V3", 0);
}
