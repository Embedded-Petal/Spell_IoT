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

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"


#define buzzer 26

int buzzerState = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
 
  digitalWrite(buzzer, LOW);
  
  Spell_iot.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  Spell_iot.registerPin("V9",[](String V)
  {
    buzzerState = V.toInt();
    digitalWrite(buzzer, buzzerState ? HIGH : LOW);
  });

}
void loop()
{
  Spell_iot.loop();
}
