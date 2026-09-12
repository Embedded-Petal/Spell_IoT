#ifndef SPELLIOT_MQTT_H
#define SPELLIOT_MQTT_H

#ifdef SPELLIOT_MQTT

#if defined(ESP8266)

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Schedule.h>
#include <Ticker.h>

#elif defined(ESP32)

#include <WiFi.h>
#include <WiFiClientSecure.h>

#endif

#include <PubSubClient.h>
#include <functional>
#include <map>

typedef std::function<void(String)> PinCallback;

class Spell_IoT;  // Forward declaration
inline Spell_IoT *instancePtr = nullptr;

#if defined(ESP32)
inline TaskHandle_t autoRunTaskHandle;
#elif defined(ESP8266)
inline Ticker autoRunTimer;
#endif

class Spell_IoT {
private:
  //WiFiClient espClient;
  WiFiClientSecure espClient;
  PubSubClient *mqttClient;
  String ssid;
  String password;
  String deviceToken;

  unsigned long lastUplinkTime;
  unsigned long lastWifiTry;
  unsigned long lastMqttTry;

  String mqtt_server = "mqtt.spelliot.com";
  const int mqtt_port = 8883;
  const char *mqtt_username = "device";

  String uplinkTopic;
  String downlinkTopic;
  String mqttClientId;

  std::map<String, PinCallback> callbacks;
  void connectWiFiNonBlocking() {
    if (WiFi.status() != WL_CONNECTED) {
      if (millis() - lastWifiTry > 10000 || lastWifiTry == 0) {
        if (lastWifiTry == 0) {
          WiFi.mode(WIFI_STA);
          WiFi.begin(ssid.c_str(), password.c_str());
        } else {
          WiFi.disconnect();
          WiFi.begin(ssid.c_str(), password.c_str());
        }
        lastWifiTry = millis();
      }
    } else {
      static bool wasConnected = false;
      if (!wasConnected) {
        Serial.println("WiFi Connected..");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        wasConnected = true;
      }
    }
  }

  void reconnectMQTTNonBlocking() {
    if (WiFi.status() == WL_CONNECTED && !mqttClient->connected()) {
      if (millis() - lastMqttTry > 5000 || lastMqttTry == 0) {
        Serial.print("Attempting MQTT connection...");

        if (mqttClient->connect(mqttClientId.c_str(), mqtt_username,
                                deviceToken.c_str())) {
          Serial.println("connected");

          // Subscribe to downlink topic upon connection
          if (mqttClient->subscribe(downlinkTopic.c_str())) {
            Serial.print("Subscribed to: ");
            Serial.println(downlinkTopic);
          } else {
            Serial.println("MQTT subscription failed");
          }
        } else {
          Serial.print("failed, rc=");
          Serial.print(mqttClient->state());
          Serial.println(" try again in 5 seconds");
        }
        lastMqttTry = millis();
      }
    }
  }

  // Static callback function that PubSubClient can call
  static void mqttCallbackFunc(char *topic, byte *payload,
                               unsigned int length) {
    if (instancePtr != nullptr) {
      instancePtr->handleMqttMessage(topic, payload, length);
    }
  }

  // Internal handler for MQTT messages
  void handleMqttMessage(char *topic, byte *payload, unsigned int length) {
    String receivedData;
    for (unsigned int i = 0; i < length; i++) {
      receivedData += (char)payload[i];
    }
    receivedData.trim();

    // Expected format: V2=1
    int separatorPosition = receivedData.indexOf('=');
    if (separatorPosition != -1) {
      String virtualPin = receivedData.substring(0, separatorPosition);
      String value = receivedData.substring(separatorPosition + 1);
      virtualPin.trim();
      value.trim();

      // Execute callback if registered
      if (callbacks.count(virtualPin)) {
        callbacks[virtualPin](value);
      }
    }
  }

public:
  Spell_IoT() {
    espClient.setInsecure();
    mqttClient = new PubSubClient(espClient);
    lastUplinkTime = 0;
    lastWifiTry = 0;
    lastMqttTry = 0;
  }

  // Begin with optional MQTT host
  void begin(String _ssid, String _password, String _token,
             String _mqtt_host = "mqtt.spelliot.com") {
    instancePtr = this;
    this->ssid = _ssid;
    this->password = _password;
    this->deviceToken = _token;
    this->mqtt_server = _mqtt_host;

    mqttClientId = String(WiFi.macAddress());
    uplinkTopic = "device/" + deviceToken + "/uplink";
    downlinkTopic = "device/" + deviceToken + "/downlink";

    mqttClient->setServer(mqtt_server.c_str(), mqtt_port);
    mqttClient->setCallback(mqttCallbackFunc);

    // Start the background task (Non-blocking)
#if defined(ESP32)
    xTaskCreatePinnedToCore(
      [](void *parameter) {
        while (true) {
          if (instancePtr != nullptr) {
            instancePtr->autoRun();
          }
          vTaskDelay(pdMS_TO_TICKS(50));
        }
      },
      "AutoRunTask", 8192, NULL, 1, &autoRunTaskHandle, 1);
#elif defined(ESP8266)
    autoRunTimer.attach_ms(50, []() {
      schedule_function([]() {
        if (instancePtr != nullptr) {
          instancePtr->autoRun();
        }
      });
    });
#endif
  }

  void autoRun() {
    connectWiFiNonBlocking();

    if (WiFi.status() == WL_CONNECTED) {
      if (!mqttClient->connected()) {
        reconnectMQTTNonBlocking();
      } else {
        mqttClient->loop();

        // Every 10 seconds, publish uplink status
        unsigned long currentMillis = millis();
        if (currentMillis - lastUplinkTime >= 10000) {
          lastUplinkTime = currentMillis;
          String payload = "status=online";
          mqttClient->publish(uplinkTopic.c_str(), payload.c_str());
        }
      }
    }
    yield();
  }

  // Dummy function for backward compatibility
  void loop() {
    // Background task handles the logic
  }

  // Register a callback for a virtual pin
  void registerPin(String pin, PinCallback cb) {
    callbacks[pin] = cb;
  }

  // Write a string value to a virtual pin
  bool write(String virtualPin, String value) {
    if (!mqttClient->connected())
      return false;
    String payload = virtualPin + "=" + value;
    return mqttClient->publish(uplinkTopic.c_str(), payload.c_str());
  }

  // Write any type to a virtual pin
  template<typename T> bool write(String pin, T value) {
    return write(pin, String(value));
  }
};

inline Spell_IoT Spell_iot;

#endif  // SPELLIOT_MQTT

#endif  // SPELLIOT_MQTT_H
