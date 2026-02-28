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

#include <Spelliot_Ledmatrix.h>

#define WIFI_SSID     "**********"
#define WIFI_PASSWORD  "***********"
#define DEVICE_TOKEN  "*********************"
#define Din 22
#define sck 21

LEDMATRIX display(Din, sck);  


uint16_t width = 6;
uint16_t spacer = 0;
String textStr = "";           // String storage (SAFE)
const char* text = "";         // char* for LED matrix

int len = 0;
String vPinRead = "V1";
void displayPrint()
{
  if (len > 3)
  {
    display.Ledmatrixscroll(width, spacer, text, 1);
    delay(500);
    display.LedMatrixClear();
    delay(50);
  }
  else if (len < 4 && len > 0)
  {
    display.LedmatrixShow(0, 0, text, 1);
    delay(50);
  }

}
void setup()
{
  Serial.begin(115200);
  display.Ledmatrixinit(7, 1 , 1); //1. brightness 2. Rotation 3. Mirror
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Spell_iot.registerPin(vPinRead, [] (String V)
  {
    textStr = V;                    // store String
    text = textStr.c_str();         // get char pointer
    len = textStr.length();         // correct length

    Serial.println(text);
    Serial.println(len);
  });
}

void loop()
{
  Spell_iot.loop();
  displayPrint();

}
