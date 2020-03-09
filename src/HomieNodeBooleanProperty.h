#ifndef HOMIENODEBOOLEANPROPERTY_H
#define HOMIENODEBOOLEANPROPERTY_H

#include <Arduino.h>
#include <errno.h>
#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeProperty.h"

class HomieNodeBooleanProperty: public HomieNodeProperty{
private:
    bool rawValue = 0;
    bool tempNewValue = 0;
    
public:
    HomieNodeBooleanProperty(String propertId, String propertyName, bool isSetable, bool isRetainable, std::function<void(void)> propChangeCallback, std::function<void(void)> errorCallback);

    virtual void onPropertyChanged(const char * value);
    virtual const char * getPropertyValue();

    bool getValueAsBool();
    void setBoolValue(bool newValue);
};

#endif //HOMIENODEBOOLEANPROPERTY_H