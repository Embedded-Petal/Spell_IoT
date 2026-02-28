#include "WiFiType.h"
#include "iotCloud.h"

String WS_HOST = "api.spelliot.com";
uint16_t WS_PORT = 8443;
String WS_PATH = "/ws-mobile";


Spell_IoT Spell_iot;
static Spell_IoT *instancePtr;
volatile long lastStatusSend = 0;
volatile static uint32_t lastWs = 0;
volatile int getIntDocker;
String airValues = "";



void Spell_IoT::begin(String ssid, String password, String token) {

  ws.dockerBegin("cloud");
  getIntDocker = ws.dockerReadInt("update", 0);
  ws.dockerEnd();

  instancePtr = this;
  this->ssid = ssid;
  this->password = password;
  this->deviceToken = token;

  for (int i = 0; i < 500; i++) {
    _lastWriteValue[i] = -1;
    _lastRGB[i] = { 0, 0, 0 };
    _lastWriteString[i] = "";
  }

  connectWiFi();
  connectWS();
  uint32_t t0 = millis();
  while (millis() - t0 < 1000) {  // 1 second handshake window
    ws.loop();
    delay(1);
  }
}

void Spell_IoT::connectWiFi() {
  WiFi.begin(ssid.c_str(), password.c_str());
  unsigned long t = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    delay(200);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected..");
    if (!ws.isConnected()) {
      connectWS();
    }
  }
}



void Spell_IoT::connectWS() {
  //ws.beginSSL(WS_HOST.c_str(), WS_PORT, WS_PATH.c_str());
  ws.begin(WS_HOST.c_str(), WS_PORT, WS_PATH.c_str());
  // Heartbeat (important for cloud)
  ws.enableHeartbeat(15000, 8000, 2);
  // STOMP event handler
  ws.onEvent(Spell_IoT::wsEvent);
}


void Spell_IoT::registerPin(String pin, PinCallback cb) {
  callbacks[pin] = cb;
}

void Spell_IoT::loop() {
  if (WiFi.status() != WL_CONNECTED)
    connectWiFi();

  // Auto reconnect WS
  if (!ws.isConnected() && WiFi.status() == WL_CONNECTED) {
    static unsigned long lastTry = 0;
    if (millis() - lastTry > 3000) {
      Serial.println("Try to reconnect");
      ws.disconnect();
      connectWS();
      lastTry = millis();
    }
  } else if (ws.isConnected() && WiFi.status() == WL_CONNECTED) {
    if (millis() - lastStatusSend >= 10000) {  // 10 seconds
      writeAck("status", "Online");
      if (getIntDocker == 1) {
        ws.dockerBegin("cloud");
        String getStringDocker = ws.dockerReadString("airStatus", "");
        for (int i = 0; i < 2; i++) {
          writeAck("airStatus", getStringDocker);
          delay(500);
        }
        ws.dockerSaveInt("update", 0);
        ws.dockerEnd();
        getIntDocker = 0;
      }
      lastStatusSend = millis();
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - lastWs >= 20) {  // 50Hz WebSocket
      ws.loop();
      lastWs = millis();
    }
  }
  yield();  // or delay(0)
}

/**************** DISPATCH ****************/

void Spell_IoT::dispatchPin(String pin, String value) {
  // if (lastValues[pin] == value) return;
  lastValues[pin] = value;

  if (isHexColor(value)) {
    int idx = pinIndex(pin);
    String val = value;
    if (val.startsWith("#")) val.remove(0, 1);

    _lastRGB[idx].r = hexByte(val.substring(0, 2));
    _lastRGB[idx].g = hexByte(val.substring(2, 4));
    _lastRGB[idx].b = hexByte(val.substring(4, 6));
  }

  if (callbacks.count(pin)) callbacks[pin](value);
}

/**************** READ ****************/

String Spell_IoT::read(String pin) {
  return lastValues[pin];
}

int Spell_IoT::readInt(String pin) {
  return lastValues[pin].toInt();
}

bool Spell_IoT::readBool(String pin) {
  return lastValues[pin] == "1";
}

Spell_IoT::RGB Spell_IoT::readRGB(String pin) {
  int idx = pinIndex(pin);
  if (idx < 0 || idx >= 500) return { 0, 0, 0 };
  return _lastRGB[idx];
}

void Spell_IoT::storeMemoryString(String keyss, String values) {
  ws.dockerBegin("cloud");
  ws.dockerSaveString(keyss.c_str(), values.c_str());
  ws.dockerEnd();
}
void Spell_IoT::storeMemoryInt(String keyss, int values) {
  ws.dockerBegin("cloud");
  ws.dockerSaveInt(keyss.c_str(), values);
  ws.dockerEnd();
}

/**************** WRITE (Unified) *************/


String Spell_IoT::urlEncode(const String &value) {
  String encoded = "";
  char c;
  char bufHex[4];
  for (int i = 0; i < value.length(); i++) {
    c = value.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else if (c == ' ') {
      encoded += "%20";
    } else {
      sprintf(bufHex, "%%%02X", c);
      encoded += bufHex;
    }
  }
  return encoded;
}

bool Spell_IoT::writeAck(String pin, String value) {
  if (!ws.isConnected()) return false;
  int idx = pinIndex(pin);
  String encodedValue = urlEncode(value);
  String msg =
    "SEND\n"
    "destination:/app/device/"
    + instancePtr->deviceToken + "\n\n" + pin + "=" + encodedValue + '\0';
  
  ws.sendTXT(msg);
  return true;
}


bool Spell_IoT::writeInternal(String pin, String value) {
  writeAck(pin, value);
  return true;
}

/**************** HELPERS ****************/

int Spell_IoT::pinIndex(String pin) {
  pin.replace("V", "");
  return pin.toInt();
}

bool Spell_IoT::isHexColor(String v) {
  if (v.startsWith("#")) v = v.substring(1);
  if (v.length() != 6) return false;
  for (int i = 0; i < 6; i++)
    if (!isxdigit(v[i])) return false;
  return true;
}

uint8_t Spell_IoT::hexByte(String h) {
  return strtol(h.c_str(), NULL, 16);
}


/*********Check For Updates********** */
void Spell_IoT::updates(String url) {
  HTTPClient http;
  http.begin(url);            // Specify the URL
  int httpCode = http.GET();  // Make the request

  if (httpCode == HTTP_CODE_OK) {  // Check for the returning code
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);
    if (canBegin) {
      Serial.println("Begin OTA update...");

      WiFiClient *client = http.getStreamPtr();
      size_t written = Update.writeStream(*client);

      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength));
      }

      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
          airValues = "Success";
          storeMemoryString("airStatus", airValues);
          storeMemoryInt("update", 1);
          delay(1000);
          ESP.restart();
        } else {
          airValues = "Update not finished? Something went wrong!";
          writeAck("airStatus", airValues);

          Serial.println("Update not finished? Something went wrong!");
          delay(1000);
        }
      } else {
        airValues = "Error Occurred. Error #: " + String(Update.getError());
        writeAck("airStatus", airValues);
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        delay(1000);
      }
    } else {
      airValues = "Not enough space to begin OTA";
      writeAck("airStatus", airValues);
      Serial.println("Not enough space to begin OTA");
      delay(1000);
    }
  } else {
    airValues = "Firmware not found on server, Error: " + String(httpCode);
    writeAck("airStatus", airValues);
    Serial.println("Firmware not found on server, Error: " + String(httpCode));
    delay(1000);
  }

  http.end();  // Close connection
}

/**************** STOMP WS ****************/

void Spell_IoT::sendSTOMP(String f) {
  f += "\n\n";
  f += '\0';
  ws.sendTXT(f);
}


void Spell_IoT::wsEvent(WStype_t type, uint8_t *payload, size_t length) {
  String msg = (char *)payload;
  switch (type) {
    case WStype_CONNECTED:
      {
        Serial.println("Petal Cloud CONNECT");
        instancePtr->sendSTOMP(
          "CONNECT\n"
          "accept-version:1.2\n"
          "host:api.spelliot.com\n"
          "heart-beat:10000,0000");
        break;
      }
     
    case WStype_TEXT:
      {
        // CONNECTED frame
        if (msg.startsWith("CONNECTED")) {
          instancePtr->sendSTOMP(
            "SUBSCRIBE\n"
            "id:sub-0\n"
            "ack:auto\n"
            "destination:/topic/device/"
            + instancePtr->deviceToken);
          break;
        }

        // MESSAGE frame
        if (msg.startsWith("MESSAGE")) {
          int idx = msg.indexOf("\n\n");
          if (idx > 0) {
            String body = msg.substring(idx + 2);
            size_t capacity = body.length() * 1.2 + 256;
            DynamicJsonDocument doc(capacity);
            if (deserializeJson(doc, body) == DeserializationError::Ok) {
              String pin = doc["pin"].as<String>();
              String val = doc["value"].as<String>();
              val.trim();
              pin.trim();
              pin.toUpperCase();
              //Serial.println(msg);
              if (pin == "AIR") {
                Serial.println("OTA!");
                instancePtr->updates(val);
              } else {
                instancePtr->dispatchPin(pin, val);
              }
            }
          }
        }
        break;
      }
  }
}


