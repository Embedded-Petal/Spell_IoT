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
#include <LiquidCrystal.h>

#define WIFI_SSID "************"
#define WIFI_PASSWORD "*************"
#define DEVICE_TOKEN "**********************"

// ---------------- LCD CONFIG ----------------
// RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

// ---------------- CLOUD STRING ----------------
String lcdText = "";

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  // LCD Initialize
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IoT Cloud");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");

  delay(2000);
  lcd.clear();

  
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Spell_iot.registerPin("V14", [](String V) {
  lcdText = V;
  Serial.print("Received String: ");
  Serial.println(lcdText);

  lcd.clear();

  // Line 1
  lcd.setCursor(0, 0);
  lcd.print(lcdText.substring(0, 16));

  // Line 2 (if text > 16 chars)
  if (lcdText.length() > 16) {
    lcd.setCursor(0, 1);
    lcd.print(lcdText.substring(16, 32));
  }
  });
}

// ---------------- LOOP ------

void loop() {
  Spell_iot.loop();
}
