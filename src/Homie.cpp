#include <Arduino.h>
#include <map>
#include "Homie.h"
#include <HomieNode.h>
#include <HomieNodeProperty.h>
#include "MQTTWrapperBase.h"

Homie::Homie(String address, uint16_t port, String username, String password, String friendlyName, String deviceId, uint8_t nodeCount):
    address(address),
    port(port), 
    username(username),
    password(password),
    friendlyName(friendlyName),
    deviceId(deviceId),
    nodesAdded(0),
    nodesCount(nodeCount) {
        nodes = (HomieNode**) malloc(nodeCount * sizeof(HomieNode*));
    }

void Homie::addNode(HomieNode * node) {
    if(nodesAdded < nodesCount) {
        nodes[nodesAdded] = node;
        nodesAdded++;
    }

}

void Homie::_init() {
    base->init(address, port, username, password);
    announceMain();
}


void Homie::onRecieve(char * topic, const char * payload) {
    char* splitted = strtok(topic, "/");
    uint8_t count = 0;
    HomieNode* nodeToUse = nullptr;
    HomieNodeProperty* propertyToUse = nullptr;
    while(splitted != 0) {
        if(count == 0) {
            if(strcmp("homie", splitted) == 0) {
                count++;
            } else {
                break;
            }
        } else if(count == 1) {
            if(strcmp(deviceId.c_str(), splitted) == 0) {
                count++;
            } else {
                break;
            }
        } else if(count == 2) {
                for(uint8_t i=0;i<nodesAdded;i++) {
                    if(strcmp(nodes[i]->nodeId.c_str(), splitted) == 0) {
                        nodeToUse = nodes[i];
                    }
                }
                if(nodeToUse != nullptr) {
                    count++;
                } else {
                    break;
                }
        } else if(count == 3) {
                propertyToUse = nodeToUse->getPropertyByName(splitted);
                if(propertyToUse == nullptr) {
                    break;
                }
                if(payload != nullptr) {
                    if(propertyToUse->isSetable) {
                        Serial.println("Setting property");
                        propertyToUse->onPropertyChanged(payload);
                        announcePropertyValue(nodeToUse->nodeId.c_str(), propertyToUse->propertyId.c_str(), propertyToUse->getPropertyValue());
                    }
                }
                break;
        }
        splitted = strtok(NULL, "/");
    }
}

void Homie::publishPropertyChange(String nodeId, String propertyId, String newValueAsString, bool isRetained) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId);
    base->publish(topic, newValueAsString, 1, isRetained);
}

String Homie::ConcatToTopic(String concatsPart1, String concatPart2) {
    String out = "";
    out.concat(concatsPart1);
    if(!concatsPart1.endsWith("/")) {
        out.concat("/");
    }
    if(concatPart2.startsWith("/")) {
        out.concat(concatPart2.substring(1, concatPart2.length()));
    } else {
        out.concat(concatPart2);
    }
    return out;
}

void Homie::announceMain() {
    announceVersion();
    announceName();
    announceImplementation();
    announceState();
    announceNodes();
    for(uint8_t i = 0;i<nodesAdded;i++) {
        announceNode(nodes[i]);
    }
    for(uint8_t i = 0;i<nodesAdded;i++) {
        subscribePropertySet(nodes[i]);
    }
}

void Homie::announceNode(HomieNode* node) {
    announceNodeName(node->nodeId.c_str(), node->name.c_str());
    announceNodeType(node->nodeId.c_str(), node->type.c_str());
    announceNodeProperties(node->nodeId.c_str(), node->getPropertiesCommaSeperated());
    for(uint8_t i = 0;i<node->propertiesAdded;i++) {
        announceProperty(node, node->properties[i]);
    }
}

void Homie::subscribePropertySet(HomieNode* node) {
    for(uint8_t i = 0;i<node->propertiesAdded;i++) {
        if(node->properties[i]->isSetable) {
            String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), node->nodeId), node->properties[i]->propertyId), HOMIE_PROPERTY_SET);
            base->registerForTopic(topic);
        }
    }
}

void Homie::announceProperty(HomieNode* node, HomieNodeProperty* property) {
    announcePropertyName(node->nodeId.c_str(), property->propertyId.c_str(), property->propertyName.c_str());
    announcePropertyDataType(node->nodeId.c_str(), property->propertyId.c_str(), property->dt_toString());
    announcePropertyIsSetable(node->nodeId.c_str(), property->propertyId.c_str(), property->isSetable);
    announcePropertyIsRetained(node->nodeId.c_str(), property->propertyId.c_str(), property->isRetained);
    if(property->formatIsSet()) {
        announcePropertyFormat(node->nodeId.c_str(), property->propertyId.c_str(), property->format.c_str());
    }
    if(property->unitIsSet()) {
        announcePropertyUnit(node->nodeId.c_str(), property->propertyId.c_str(), property->unit.c_str());
    }
    announcePropertyValue(node->nodeId.c_str(), property->propertyId.c_str(), property->getPropertyValue());
}


void Homie::announceVersion() {
    String topic = ConcatToTopic(ConcatToTopic("homie", deviceId), HOMIE_VERSION_TOPIC);
    base->publish(topic, HOMIE_CONVENTION_VERSION, 1 ,true);
}
void Homie::announceName() {
    String topic = ConcatToTopic(ConcatToTopic("homie", deviceId), HOMIE_NAME_TOPIC);
    base->publish(topic, friendlyName, 1 ,true);
}
void Homie::announceNodes() {
    String topic = ConcatToTopic(ConcatToTopic("homie", deviceId), HOMIE_NODES_TOPIC);
    String nodes = "";
    for(uint8_t i=0;i<nodesCount;i++) {
        nodes += this->nodes[i]->nodeId;
        if(i+1 != nodesCount) {
            nodes += ",";
        }
    }
    base->publish(topic, nodes, 1 ,true);
}
void Homie::announceState() {
    String topic = ConcatToTopic(ConcatToTopic("homie", deviceId), HOMIE_STATE_TOPIC);
    if(state) {
        base->publish(topic, HOMIE_STATE_READY, 1 ,true);
    } else {
        base->publish(topic, HOMIE_STATE_INIT, 1 ,true);
    }
}
void Homie::announceImplementation() {
    String topic = ConcatToTopic(ConcatToTopic("homie", deviceId), HOMIE_IMPLEMENTATION_TOPIC);
    base->publish(topic, HOMIE_IMPLEMENTATION, 1 ,true);
}
void Homie::announceNodeName(const char * nodeId, const char * nodeName) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), HOMIE_NAME_TOPIC);
    base->publish(topic, nodeName, 1 ,true);
}
void Homie::announceNodeType(const char * nodeId, const char * nodeType) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), HOMIE_NODE_TYPE_TOPIC);
    base->publish(topic, nodeType, 1 ,true);
}
void Homie::announceNodeProperties(const char * nodeId, const char * nodeProps) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), HOMIE_PROPERTIES_TOPIC);
    base->publish(topic, nodeProps, 1 ,true);
}
void Homie::announcePropertyName(const char * nodeId, const char * propertyId, const char * propertyName) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId), HOMIE_NAME_TOPIC);
    base->publish(topic, propertyName, 1 ,true);
}
void Homie::announcePropertyValue(const char * nodeId, const char * propertyId, const char * propertyValue) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId);
    base->publish(topic, propertyValue, 1 ,true);
}
void Homie::announcePropertyDataType(const char * nodeId, const char * propertyId, const char * propertyDataType) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId), HOMIE_PROPERTY_DATATYPE);
    base->publish(topic, propertyDataType, 1 ,true);
}
void Homie::announcePropertyUnit(const char * nodeId, const char * propertyId, const char * propertyUnit) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId), HOMIE_PROPERTY_UNIT);
    base->publish(topic, propertyUnit, 1 ,true);
}

void Homie::announcePropertyFormat(const char * nodeId, const char * propertyId, const char * propertyFormat) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId), HOMIE_PROPERTY_FORMAT);
    base->publish(topic, propertyFormat, 1 ,true);
}
void Homie::announcePropertyIsSetable(const char * nodeId, const char * propertyId, bool isSetable) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId), HOMIE_PROPERTY_SETTABLE);
    if(isSetable) {
        base->publish(topic, "true", 1 ,true);
    } else {
        base->publish(topic, "false", 1 ,true);
    }
}
void Homie::announcePropertyIsRetained(const char * nodeId, const char * propertyId, bool isRetained) {
    String topic = ConcatToTopic(ConcatToTopic(ConcatToTopic(ConcatToTopic("homie", deviceId), nodeId), propertyId), HOMIE_PROPERTY_RETAINED);
    if(isRetained) {
        base->publish(topic, "true", 1 ,true);
    } else {
        base->publish(topic, "false", 1 ,true);
    }
}