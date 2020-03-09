#ifndef HOMIENODEINTEGERPROPERTY_H
#define HOMIENODEINTEGERPROPERTY_H

#include <Arduino.h>
#include <errno.h>
#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeProperty.h"

class HomieNodeIntegerProperty: public HomieNodeProperty{
private:
    intmax_t rawValue = 0;
    intmax_t tempNewValue = 0;
    char buffer[33];
public:
    HomieNodeIntegerProperty(String propertId, String propertyName, String format, String unit, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback, std::function<void(void)> errorCallback);

    virtual void onPropertyChanged(const char * value);
    virtual const char * getPropertyValue();

    intmax_t getValueAsIntMax();
    void setIntegerValue(intmax_t newValue);
};

#endif //HOMIENODEINTEGERPROPERTY_H