#ifndef HOMIE_H
#define HOMIE_H
#include <Arduino.h>
#include <map>

#define HOMIE_CONVENTION_VERSION "3.0"

#define HOMIE_STATE_READY "ready"
#define HOMIE_STATE_INIT "init"
#define HOMIE_ROOT_TOPIC "homie"

#define HOMIE_VERSION_TOPIC "$homie"
#define HOMIE_NAME_TOPIC "$name"
#define HOMIE_STATE_TOPIC "$state"
#define HOMIE_NODES_TOPIC "$nodes"
#define HOMIE_IMPLEMENTATION_TOPIC "$implementation"
#define HOMIE_PROPERTIES_TOPIC "$properties"

#define HOMIE_PROPERTY_DATATYPE "$datatype"
#define HOMIE_PROPERTY_FORMAT "$format"
#define HOMIE_PROPERTY_SETTABLE "$settable"
#define HOMIE_PROPERTY_RETAINED "$retained"
#define HOMIE_PROPERTY_UNIT "$unit"

#define HOMIE_PROPERTY_SET "set"

#define HOMIE_NODE_TYPE_TOPIC "$type"

#define HOMIE_IMPLEMENTATION "shl"

class HomieNode;
class HomieNodeProperty;
class MQTTWrapperBase;
class Homie {
    private:
        String address;
        uint16_t port;
        String username;
        String password;
        String friendlyName;
        String deviceId;
        bool state;
        HomieNode** nodes;
        uint8_t nodesAdded;
        uint8_t nodesCount;
        MQTTWrapperBase* base;
        void _init();
        void announceMain();
        void announceVersion();
        void announceName();
        void announceNodes();
        void announceState();
        void announceImplementation();
        void announceNode(HomieNode* node);
        void announceProperty(HomieNode* node, HomieNodeProperty* property);
        void subscribePropertySet(HomieNode* node);
        void announceNodeName(const char * nodeId, const char * nodeName);
        void announceNodeType(const char * nodeId, const char * nodeType);
        void announceNodeProperties(const char * nodeId, const char * nodeProps);
        void announcePropertyName(const char * nodeId, const char * propertyId, const char * propertyName);
        void announcePropertyValue(const char * nodeId, const char * propertyId, const char * propertyValue);
        void announcePropertyDataType(const char * nodeId, const char * propertyId, const char * propertyDataType);
        void announcePropertyUnit(const char * nodeId, const char * propertyId, const char * propertyUnit);
        void announcePropertyFormat(const char * nodeId, const char * propertyId, const char * propertyFormat);
        void announcePropertyIsSetable(const char * nodeId, const char * propertyId, bool isSetable);
        void announcePropertyIsRetained(const char * nodeId, const char * propertyId, bool isRetained);
        String ConcatToTopic(String concatsPart1, String concatPart2);
        
    public:
        Homie(String address, uint16_t port, String username, String password, String firendlyName, String deviceId, uint8_t nodeCount);
        void addNode(HomieNode* node);
        template<typename MqttWrapper> void init() {
            static_assert(std::is_base_of<MQTTWrapperBase, MqttWrapper>{}, "MqttWrappe template parameter must be derived from MqttWrapperBase");
            base = new MqttWrapper(this);
            _init();
        }
        
        void onRecieve(char *, const char * payload);
        void publishPropertyChange(String nodeId, String propertyId, String newValueAsString, bool isRetained);
};

#endif // HOMIE_H