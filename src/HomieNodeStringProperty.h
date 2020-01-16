#ifndef HOMIENODESTRINGPROPERTY_H
#define HOMIENODESTRINGPROPERTY_H


#include <Arduino.h>
#include "HomieNodeProperty.h"
#include "HomieNode.h"

class HomieNodeStringProperty: public HomieNodeProperty{
    private:
    const char * value = "";
public:
    HomieNodeStringProperty(String propertId, String propertyName, String format, String unit, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback);

    void onPropertyChanged(const char * value);
    const char * getPropertyValue();

    String getValueAsString();
    void setStringVlaue(const char * value);

};

#endif //HOMIENODESTRINGPROPERTY_H