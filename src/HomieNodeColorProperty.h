#ifndef HOMIENODECOLORPROPERTY_H
#define HOMIENODECOLORPROPERTY_H

#include <Arduino.h>
#include <errno.h>

#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeProperty.h"

class HomieNodeColorProperty: public HomieNodeProperty{
private:
    uint8_t r_h;
    uint8_t g_s;
    uint8_t b_v;
    uint8_t temp_r_h;
    uint8_t temp_g_s;
    uint8_t temp_b_v;
    char buffer[12];
    
public:
    HomieNodeColorProperty(String propertId, String propertyName, bool isSetable, bool isRetainable, bool isHsv, std::function<void(void)> propChangeCallback);

    virtual void onPropertyChanged(const char * value);
    virtual const char * getPropertyValue();

    uint8_t getR_HValue();
    uint8_t getG_SValue();
    uint8_t getB_VValue();
    void setRGB_HSVValue(uint8_t r_h, uint8_t g_s, uint8_t b_v);
};

#endif //HOMIENODECOLORPROPERTY_H