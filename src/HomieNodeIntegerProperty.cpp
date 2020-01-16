#include <Arduino.h>
#include <errno.h>
#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeIntegerProperty.h"

    HomieNodeIntegerProperty::HomieNodeIntegerProperty(String propertId, String propertyName, String format, String unit, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback)
    {
        datatype = DT_INTEGER;
        this->propertyId = propertId;
        this->propertyName = propertyName;
        this->format = format;
        this->unit = unit;
        this->isSetable = isSetable;
        this->isRetained = isRetainable;
        this->propChangeCallback = propChangeCallback;

    }

    void HomieNodeIntegerProperty::onPropertyChanged(const char * value) {
        
        this->tempNewValue = atoll(value);
        if(errno == ERANGE) {
            errno = 0;

        } else {
            this->rawValue = this->tempNewValue;
            this->propChangeCallback();
        }
    }

    const char * HomieNodeIntegerProperty::getPropertyValue() {
        return ltoa(rawValue, buffer, 10);
    }

    intmax_t HomieNodeIntegerProperty::getValueAsIntMax() {
        return rawValue;
    }

    void HomieNodeIntegerProperty::setIntegerValue(intmax_t newValue) {
        this->rawValue = newValue;
        this->publish();
    }
