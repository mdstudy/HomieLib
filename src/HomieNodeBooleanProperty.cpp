#include <Arduino.h>
#include <errno.h>
#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeBooleanProperty.h"


    HomieNodeBooleanProperty::HomieNodeBooleanProperty(String propertId, String propertyName, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback, std::function<void(void)> errorCallback)
    {
        datatype = DT_BOOLEAN;
        this->propertyId = propertId;
        this->propertyName = propertyName;
        this->isSetable = isSetable;
        this->isRetained = isRetainable;
        this->propChangeCallback = propChangeCallback;
        this->errorCallback = errorCallback;
    }

    void HomieNodeBooleanProperty::onPropertyChanged(const char * value) {
        if(strcmp("true", value) == 0) {
            this->rawValue = true;
        } else if(strcmp("false", value)) {
            this->rawValue = false;
        } else {
            if(this->errorCallback != NULL) {
                this->errorCallback();
            }
        }
        this->propChangeCallback();
    }

    const char * HomieNodeBooleanProperty::getPropertyValue() {
        if(rawValue) {
            return "true";
        } else {
            return "false";
        }
    }

    bool HomieNodeBooleanProperty::getValueAsBool() {
        return rawValue;
    }

    void HomieNodeBooleanProperty::setBoolValue(bool newValue) {
        this->rawValue = newValue;
        this->publish();
    }