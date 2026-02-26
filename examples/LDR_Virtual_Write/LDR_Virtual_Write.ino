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

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********""

#define LDR 33


void setup() {
  Serial.begin(115200);
  pinMode(LDR, INPUT);
 
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Spell_iot.loop();
  float a = analogRead(LDR);
  int percentage = (a / 4095) * 100 ;
  Spell_iot.write("V5", percentage);
  Serial.println(percentage);
  delay(100);
 
}
