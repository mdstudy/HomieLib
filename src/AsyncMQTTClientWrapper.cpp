#ifdef ESP32
#include <Arduino.h>
#include "AsyncMQTTClientWrapper.h"
#include <AsyncMqttClient.h>
//#include <functional>

AsyncMQTTClientWrapper *AsyncMQTTClientWrapper::self = NULL;

void _onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    char buffer[len+1];
    strncpy(buffer, payload, len);
    buffer[len] = '\0';
    Serial.printf("Topic: %s\nRecieved: %s\n", topic, buffer);
    AsyncMQTTClientWrapper::self->messageRecieved(topic, buffer);
}

void _onConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT");
    AsyncMQTTClientWrapper::self->ready = true;
}

void _onDisconnect(AsyncMqttClientDisconnectReason reason) {
    AsyncMQTTClientWrapper::self->ready = false;
}

//class AsyncMQTTClientWrapper : public MQTTWrapperBase{
//   private:
//    public:
        AsyncMQTTClientWrapper::AsyncMQTTClientWrapper(Homie* homie):MQTTWrapperBase(homie) {
            ready = false;
            AsyncMQTTClientWrapper::self = this;
        }

        void AsyncMQTTClientWrapper::init(String address, uint16_t port, String username, String password) {
            mqttClient.onMessage(_onMessage);
            mqttClient.onConnect(_onConnect);
            mqttClient.onDisconnect(_onDisconnect);
            Serial.printf("Connecting to %s:%d %s %s\n", address.c_str(), port, username.c_str(), password.c_str());
            mqttClient.setServer(address.c_str(), port);
            mqttClient.setClientId("test");
            if(username.length() > 0) {
                mqttClient.setCredentials(username.c_str(), password.c_str());
            }
            mqttClient.connect();
            Serial.println("inited");
            while(!ready) {
                delay(500);
                Serial.println("Connecting MQTT");
            }
        }

        void AsyncMQTTClientWrapper::reconnect() {
            if(!mqttClient.connected()) {
                mqttClient.connect();
            }
        }

        void AsyncMQTTClientWrapper::publish(String topic, String payload, uint8_t qos, bool retain) {
            Serial.printf("Publishing to %s\n Payload: %s\n", topic.c_str(), payload.c_str());
            mqttClient.publish(topic.c_str(), qos, retain, payload.c_str(), payload.length());
        }

        void AsyncMQTTClientWrapper::setLWT(String topic, String payload) {
            mqttClient.setWill(topic.c_str(), 1, true, payload.c_str(), payload.length());
        }

        void AsyncMQTTClientWrapper::registerForTopic(String topic) {
            mqttClient.subscribe(topic.c_str(), 1);
        }

        
//};
#endif