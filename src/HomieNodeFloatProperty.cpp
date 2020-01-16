#include <Arduino.h>
#include <errno.h>
#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeFloatProperty.h"

    HomieNodeFloatProperty::HomieNodeFloatProperty(String propertId, String propertyName, String format, String unit, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback)
    {
        datatype = DT_FLOAT;
        this->propertyId = propertId;
        this->propertyName = propertyName;
        this->format = format;
        this->unit = unit;
        this->isSetable = isSetable;
        this->isRetained = isRetainable;
        this->propChangeCallback = propChangeCallback;

    }

    void HomieNodeFloatProperty::onPropertyChanged(const char * value) {
        this->tempNewValue = strtof(value, NULL);
        if(errno == ERANGE) {
            errno = 0;
        } else {
            this->rawValue = this->tempNewValue;
            this->propChangeCallback();
        }
    }

    const char * HomieNodeFloatProperty::getPropertyValue() {
        return ltoa(rawValue, buffer, 10);
    }

    float HomieNodeFloatProperty::getValueAsFloat() {
        return rawValue;
    }

    void HomieNodeFloatProperty::setFloatValue(float newValue) {
        this->rawValue = newValue;
        this->publish();
    }
