#ifndef IOTCLOUD_H
#define IOTCLOUD_H

#include "Spelliot_Config.h"
#include <Arduino.h>
#include <WiFi.h>
#include "WebSocketsClient.h"
#include <ArduinoJson.h>
#include <map>
#include <functional>
#include <Update.h>
#include <Ticker.h>

typedef std::function<void(String)> PinCallback;

class Spell_IoT {

  public:
    struct RGB {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };

    WebSocketsClient ws;
    std::map<String, PinCallback> callbacks;
    std::map<String, String> lastValues;

   
    // Generic template (last)
    template<typename T>
    bool write(String pin, T value) {
        return writeInternal(pin, String(value));
    }


    String ssid, password, deviceToken;

    void begin(String ssid, String password, String token);
    void loop();
    void autoRun();
    
    // READ
    String read(String pin);
    int readInt(String pin);
    bool readBool(String pin);
    RGB readRGB(String pin);
    bool writeAck(String pin, String value);
    bool Status();
   
    // CALLBACK
    void registerPin(String pin, PinCallback cb);
    void writeRawWS(String pin, String value);
    void storeMemoryString(String keyss, String values);
    void storeMemoryInt(String keyss, int values);

  private:
    
    bool writeInternal(String pin, String value);
    void dispatchPin(String pin, String value);
    void sendSTOMP(String f);
    void connectWiFi();
    void connectWS();
    static void tick();
    void loopInternal();
    int pinIndex(String pin);

    bool isHexColor(String v);
    uint8_t hexByte(String h);

    static void wsEvent(WStype_t type, uint8_t *payload, size_t length);

    int _lastWriteValue[500];
    String _lastWriteString[500];
    RGB _lastRGB[500];
    String urlEncode(const String &value);
    void updates(String url);
    
};

extern Spell_IoT Spell_iot;
extern Ticker ticker;


#endif


