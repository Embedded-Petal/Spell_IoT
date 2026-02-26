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

#define WIFI_SSID "*********"
#define WIFI_PASSWORD "************"
#define DEVICE_TOKEN "**************"

// Define the joystick pins
#define JOYSTICK_X_PIN 4
#define JOYSTICK_Y_PIN 15

String vPinX = "V1";
String vPinY = "V2";
void setup() {
  // Initialize the serial communication
  Serial.begin(115200);

  // Set joystick pins as input
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  Spell_iot.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Serial.println("Dual-Axis Joystick Data Monitoring");
}

void loop() {
  Spell_iot.loop();
  // Read analog values from the joystick
  int xValue = analogRead(JOYSTICK_X_PIN);
  int yValue = analogRead(JOYSTICK_Y_PIN);

  // Map the values to 8-bit ADC (0-255)
  int xMapped = map(xValue, 0, 4095, 0, 255);
  int yMapped = map(yValue, 0, 4095, 0, 255);

  // Print the mapped values
  Serial.print("X-Axis : ");
  Serial.println(xMapped);
  Serial.print("Y-Axis : ");
  Serial.println(yMapped);
  Spell_iot.write(vPinX, xMapped);
  Spell_iot.write(vPinY, yMapped);
  delay(300); // Delay for stability
}
