#include <Arduino.h>
#include <errno.h>

#include "HomieNode.h"
#include "Homie.h"
#include "HomieNodeColorProperty.h"

    HomieNodeColorProperty::HomieNodeColorProperty(String propertId, String propertyName, bool isSetable, bool isRetainable, bool isHsv, std::function<void(void)> propChangeCallback)
    {
        datatype = DT_COLOR;
        this->propertyId = propertId;
        this->propertyName = propertyName;
        this->isSetable = isSetable;
        this->isRetained = isRetainable;
        if(isHsv) {
            this->format = "hsv";
        } else {
            this->format = "rgb";
        }
        this->propChangeCallback = propChangeCallback;
    }

    void HomieNodeColorProperty::onPropertyChanged(const char * value) {
        char * token = strtok(strdup(value), ",");
        uint8_t count = 0;
        errno = 0;
        while(token != NULL) {
            if(count == 0) {
                temp_r_h = strtoul(token, NULL, 10);
                Serial.printf("Color R: %d\n", temp_r_h);
            }
            if(count == 1) {
                temp_g_s = strtoul(token, NULL, 10);
                Serial.printf("Color G: %d\n", temp_g_s);
            }
            if(count == 2) {
                temp_b_v = strtoul(token, NULL, 10);
                Serial.printf("Color B: %d\n", temp_b_v);
            }
            if(errno == ERANGE) {
                errno = 0;
                Serial.printf("Error: %d\n", errno);
                break;
            } else {
                count++;
            }
            token = strtok(NULL, ",");
        }
        if(count == 3) {
            r_h = temp_r_h;
            g_s = temp_g_s;
            b_v = temp_b_v;
            this->propChangeCallback();
        }
        
    }

    const char * HomieNodeColorProperty::getPropertyValue() {
        sprintf(buffer, "%d,%d,%d", r_h, g_s, b_v);
        return buffer;
    }

    uint8_t HomieNodeColorProperty::getR_HValue() {
        return r_h;
    }

    uint8_t HomieNodeColorProperty::getG_SValue() {
        return g_s;
    }

    uint8_t HomieNodeColorProperty::getB_VValue() {
        return b_v;
    }

    void HomieNodeColorProperty::setRGB_HSVValue(uint8_t r_h, uint8_t g_s, uint8_t b_v) {
        this->r_h = r_h;
        this->g_s = g_s;
        this->b_v = b_v;
        this->publish();
    }