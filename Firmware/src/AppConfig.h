#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "consts.h"
#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>

typedef uint8_t AppConfig_serial;

class AppConfig {
    public:
    AppConfig();
    void storeToFlash();
    void loadFromFlash();
    void setChannel(uint8_t val);
    void setCC(uint8_t val);
    void setValueMin(uint8_t val);
    void setValueMax(uint8_t val);
    uint8_t getChannel();
    uint8_t getCC();
    uint8_t getValueMin();
    uint8_t getValueMax();

    private:
    void _loadPanic();
    uint8_t channel;
    uint8_t cc;
    uint8_t valueMin;
    uint8_t valueMax;
};

#endif