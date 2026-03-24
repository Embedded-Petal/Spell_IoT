
#include <Wire.h>
#include "MAX30102.h"
#include "Pulse.h"
#include <Spell_IoT.h>

#define WIFI_SSID "******************"
#define WIFI_Password "***************"
#define DEVICE_TOKEN "************************"
// === MAX30102 and Pulse Setup ===
MAX30102 sensor;
Pulse pulseIR;
Pulse pulseRed;
MAFilter bpmFilter;

int SPO2f = 0;
int beatAvg = 0;
int nofinger = 0;

// === Timing Variables ===
unsigned long lastBeat = 0;
unsigned long lastSendTime = 0;

void readSensor() {
  
  if (!sensor.available()) return;

  uint32_t irValue = sensor.getIR();
  uint32_t redValue = sensor.getRed();
  sensor.nextSample();

  if (irValue < 5000) {
    Serial.println("Place your finger on the sensor...");
    if (nofinger == 0) {
      Spell_iot.write("V1", "0");
      Spell_iot.write("V0", "0");
      nofinger = 1;
    }
    return;
  }

  long now = millis();
  int16_t IR_signal = pulseIR.dc_filter(irValue);
  int16_t Red_signal = pulseRed.dc_filter(redValue);

  bool beatDetected = pulseIR.isBeat(pulseIR.ma_filter(IR_signal)) || pulseRed.isBeat(pulseRed.ma_filter(Red_signal));

  if (beatDetected) {
    long bpm = 60000 / (now - lastBeat);
    lastBeat = now;

    if (bpm > 40 && bpm < 180) {  // valid BPM range
      beatAvg = bpmFilter.filter((int16_t)bpm);

      // Compute SpO2
      long numerator = (pulseRed.avgAC() * pulseIR.avgDC()) / 256;
      long denominator = (pulseRed.avgDC() * pulseIR.avgAC()) / 256;
      int RX100 = (denominator > 0) ? (numerator * 100) / denominator : 999;
      SPO2f = constrain((10400 - RX100 * 17 + 50) / 100, 70, 100);

      Serial.print("BPM: ");
      Serial.print(beatAvg);
      Serial.print("  SpO2: ");
      Serial.println(SPO2f);
      Spell_iot.write("V1", beatAvg);  // BPM
      Spell_iot.write("V0", SPO2f);    // SpO2
    }
    if (nofinger == 1) {
      nofinger = 0;
    }
  }
  
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Spell_iot.begin(WIFI_SSID, WIFI_Password, DEVICE_TOKEN);
  delay(4000);
  Wire.begin(21, 22);     // SDA, SCL for ESP32
  Wire.setClock(100000);  
  Wire.setTimeout(3000);  

  Serial.println("Initializing MAX30102 sensor...");
  if (!sensor.begin()) {
    Serial.println("MAX30102 not found! Check wiring and power.");
    while (1)
      ;
  }
  sensor.setup();
  Serial.println("MAX30102 initialized successfully!");
}

void loop() {
  sensor.check();

  readSensor();
  Spell_iot.loop();
}
