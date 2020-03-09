#ifndef HOMIENODEFLOATPROPERTY_H
#define HOMIENODEFLOATPROPERTY_H


#include <Arduino.h>
#include <errno.h>
#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeProperty.h"

class HomieNodeFloatProperty: public HomieNodeProperty{
private:
    float rawValue = 0;
    float tempNewValue = 0;
    char buffer[33];
public:
    HomieNodeFloatProperty(String propertId, String propertyName, String format, String unit, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback, std::function<void(void)> errorCallback);

    virtual void onPropertyChanged(const char * value);
    virtual const char * getPropertyValue();

    float getValueAsFloat();
    void setFloatValue(float newValue);
};

#endif //HOMIENODEFLOATPROPERTY_H