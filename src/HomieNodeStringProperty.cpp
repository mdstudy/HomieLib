#include <Arduino.h>
#include "HomieNodeStringProperty.h"
#include "HomieNode.h"

    HomieNodeStringProperty::HomieNodeStringProperty(String propertId, String propertyName, String format, String unit, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback, std::function<void(void)> errorCallback)
    {
        datatype = DT_STRING;
        this->propertyId = propertId;
        this->propertyName = propertyName;
        this->format = format;
        this->unit = unit;
        this->isSetable = isSetable;
        this->isRetained = isRetainable;
        this->propChangeCallback = propChangeCallback;
        this->errorCallback = errorCallback;

    }

    void HomieNodeStringProperty::onPropertyChanged(const char * value) {
        //generally no error here as we always have strings
        this->value = strdup(value);
        this->propChangeCallback();
    }

    const char * HomieNodeStringProperty::getPropertyValue() {
        return value;
    }

    String HomieNodeStringProperty::getValueAsString() {
        return value;
    }

    void HomieNodeStringProperty::setStringVlaue(const char * value) {
        this->value = strdup(value);
        this->publish();
    }
