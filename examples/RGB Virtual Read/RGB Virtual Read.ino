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
#include <FastLED.h>

#define WIFI_SSID "*************"
#define WIFI_PASSWORD "*******************"
#define DEVICE_TOKEN "******************************"

#define LED_PIN 32
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

// RGB CACHE
int r = 0, g = 0, b = 0;


void rgb()
{
  auto c = Spell_iot.readRGB("V13");
  // Only update if changed
  if (c.r != r || c.g != g || c.b != b) {
    r = c.r;
    g = c.g;
    b = c.b;
    Serial.printf("RGB => %d, %d, %d\n", r, g, b);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(r, g, b); // Set the LED color using HSV (Hue, Saturation, Brightness)
    }
    FastLED.show();  // Update the LED strip
    delay(50);
  }
}

void setup() {
  Serial.begin(115200);
  // Initialize the RGB LED strip
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Spell_iot.loop();
  rgb();
}
