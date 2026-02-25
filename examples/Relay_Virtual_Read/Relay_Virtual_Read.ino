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


#define relay1 25
#define relay2 33

int relay1State = 0, relay2State = 0;

void setup()
{
  //Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
 
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  
  Spell_iot.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  Spell_iot.registerPin("V9",[](String V)
  {
    relay1State = V.toInt();
    digitalWrite(relay1, relay1State ? HIGH : LOW);
  });
  Spell_iot.registerPin("V10",[](String V)
  {
    relay2State = V.toInt();
    digitalWrite(relay2, relay2State ? HIGH : LOW);
  });

}
void loop()
{
  Spell_iot.loop();
}
