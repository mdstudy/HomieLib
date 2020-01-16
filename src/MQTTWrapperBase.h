#ifndef MQTTWRAPPERBASE_H
#define MQTTWRAPPERBASE_H

#include <Arduino.h>
#include <Homie.h>

class MQTTWrapperBase {
    private:
        Homie* homie;
    public:
        MQTTWrapperBase(Homie* homie): homie(homie) {};
        virtual void init(String address, uint16_t port, String username, String password) = 0;
        virtual void publish(String topic, String payload, uint8_t qos, bool retain) = 0;
        virtual void setLWT(String topic, String payload) = 0;
        virtual void registerForTopic(String topic) = 0;
        virtual void reconnect() = 0;
        void messageRecieved(char * topic, const char * payload) {
            homie->onRecieve(topic, payload);
        };
        bool ready;
};

#endif // MQTTWRAPPERBASE_H