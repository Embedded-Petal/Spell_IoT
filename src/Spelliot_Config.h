#ifndef SPELLIOT_CONFIG_H
#define SPELLIOT_CONFIG_H


/************* BOARD AUTO DETECTION *************/
#if defined(ESP32)
  #define SPELL_IOT_BOARD_ESP32
#elif defined(ESP8266)
  #define SPELL_IOT_BOARD_ESP8266
#else
  #warning "SPELL_IoT supports only ESP32 and ESP8266"
#endif

/************* COMMON LIBS *************/
#include <Arduino.h>
#include <ArduinoJson.h>

/************* BOARD SPECIFIC LIBS *************/
#ifdef SPELL_IOT_BOARD_ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

#ifdef SPELL_IOT_BOARD_ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#endif



