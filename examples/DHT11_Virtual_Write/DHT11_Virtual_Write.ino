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
#include <DHT.h>

#define WIFI_SSID "********"
#define WIFI_Password "*************"
#define DEVICE_TOKEN "********************"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void dhtRead() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read From DHT11 Sensor! ");
    return;
  }

  Spell_iot.write("V1", temperature);
  Spell_iot.write("V2", humidity);
  delay(1000);
}
void setup() {
  // put your setup code here, to run once:
  dht.begin();
  Serial.begin(115200);
  Spell_iot.begin(WIFI_SSID, WIFI_Password, DEVICE_TOKEN);
}

void loop() {
  // put your main code here, to run repeatedly:
  dhtRead();
  Spell_iot.loop();
}
