#ifndef HOMIENODE_H
#define HOMIENODE_H

#include <Arduino.h>
#include <Homie.h>

class HomieNodeProperty;
class HomieNode {
    private:
    public:
        Homie* homie;
        String nodeId;
        String name;
        String type;
        HomieNodeProperty** properties;
        uint8_t propertyCount;
        uint8_t propertiesAdded;
        HomieNode(Homie* homie, String nodeId, String name, String type, uint8_t propertyCount);
        void addProperty(HomieNodeProperty* property);
        
        char * getPropertiesCommaSeperated();
        
        HomieNodeProperty* getPropertyByName(const char * name);

};

#endif //HOMIENODE_H


