#define SKYLINK_FIRMWARE_VERSION "1.0.0"
#define SPELLIOT_UPDATE
#include <Spell_IoT.h>


#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"


#define LED1 12
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
  if (LEDState1 == 1 ) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
}  


