#ifndef HOMIENODEPROPERTY_H
#define HOMIENODEPROPERTY_H
#include <Arduino.h>
#include <functional>
#include <HomieNode.h>

enum PropertyDataType
 {
    DT_STRING, 
    DT_INTEGER,
    DT_FLOAT,
    DT_BOOLEAN,
    DT_ENUM,
    DT_COLOR
};

class HomieNodeProperty {
    public:
        virtual void onPropertyChanged(const char * value) = 0;
        bool isSetable;
        bool isRetained;
        String propertyId;
        String propertyName;
        PropertyDataType datatype;
        String format;
        String unit;
        virtual const char * getPropertyValue() = 0;
        HomieNode* parent;
        std::function<void(void)> propChangeCallback;
        std::function<void(void)> errorCallback;
        bool formatIsSet() {
            return format != nullptr && format.length() > 0;
        }

        bool unitIsSet() {
            return unit != nullptr && unit.length() > 0;
        }
        void setParent(HomieNode* parent) {
            this->parent = parent;
        }

        const char * dt_toString() {
            switch(datatype) {
                case DT_STRING:
                    return "string";
                case DT_INTEGER:
                    return "integer";
                case DT_FLOAT:
                    return "float";
                case DT_BOOLEAN:
                    return "boolean";
                case DT_ENUM:
                    return "enum";
                case DT_COLOR:
                    return "color";
            }
            return "string";
        };

    protected:
        void publish() {
            this->parent->homie->publishPropertyChange(this->parent->nodeId, this->propertyId, getPropertyValue(), isRetained);
        };
};

#endif //HOMIENODEPROPERTY_H