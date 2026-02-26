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
#include <ESP32Servo.h>

#define WIFI_SSID     "*******"
#define WIFI_PASSWORD  "************"
#define DEVICE_TOKEN  "*********************S"

#define Servo_pin 32
String vPinRead = "V1";
Servo servo;
int degree = 0;

void setup() {
  Serial.begin(115200);
  servo.attach(Servo_pin);
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Spell_iot.registerPin(vPinRead, [](String v) {
  degree = v.toInt();
  servo.write(degree);
  Serial.println("Servo Angle - " + String(degree));
  });
}

void loop() {
  Spell_iot.loop();
  
}  
