#include <Arduino.h>
#include <HomieNode.h>
#include <Homie.h>
#include <HomieNodeProperty.h>

HomieNode::HomieNode(Homie* homie, String nodeId, String name, String type, uint8_t propertyCount):
    homie(homie),
    nodeId(nodeId),
    name(name),
    type(type),
    propertyCount(propertyCount) {
        this->properties = (HomieNodeProperty**) malloc(sizeof(HomieNodeProperty*)*propertyCount);
    }

void HomieNode::addProperty(HomieNodeProperty* property) {
    if(propertiesAdded < propertyCount) {
        this->properties[propertiesAdded] = property;
        property->setParent(this);
        propertiesAdded++;
    }
}

char * HomieNode::getPropertiesCommaSeperated() {
    String ret = "";
    for(uint8_t i=0;i<propertyCount;i++) {
        if(this->properties[i] != nullptr) {
            ret.concat(this->properties[i]->propertyId);
            ret.concat(",");
        }
    }
    if(ret.length() > 0) {
        ret = ret.substring(0, ret.length()-1);
    }
    return strdup(ret.c_str());
}

HomieNodeProperty* HomieNode::getPropertyByName(const char * name) {
    for(uint8_t i=0;i<propertyCount;i++) {
        if(this->properties[i] != nullptr) {
            if(strcmp(name, this->properties[i]->propertyId.c_str()) == 0) {
                Serial.println("Found property");
                return this->properties[i];
            }
        }
    }
    return nullptr;
}