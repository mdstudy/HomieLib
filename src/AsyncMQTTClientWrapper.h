#ifdef ESP32
#ifndef ASYNCMQTTTCLIENWRAPPER_H
#define ASYNCMQTTTCLIENWRAPPER_H
#include <Arduino.h>
#include "MQTTWrapperBase.h"
#include <AsyncMqttClient.h>

class AsyncMQTTClientWrapper : public MQTTWrapperBase {
    private:
        AsyncMqttClient mqttClient;
    public:
        static AsyncMQTTClientWrapper *self;
        AsyncMQTTClientWrapper(Homie* homie);
        virtual void init(String address, uint16_t port, String username, String password);
        virtual void publish(String topic, String payload, uint8_t qos, bool retain);
        virtual void setLWT(String topic, String payload);
        virtual void registerForTopic(String topic);
        virtual void reconnect();
};
#endif //ASYNCMQTTTCLIENWRAPPER_H
#endif //ESP32
