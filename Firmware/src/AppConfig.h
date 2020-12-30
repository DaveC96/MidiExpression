#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "consts.h"
#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>

typedef uint8_t AppConfig_serial;

struct AppConfig {
    uint8_t channel;
    uint8_t cc;
    uint8_t valueMin;
    uint8_t valueMax;
};

void storeToFlash   (AppConfig* config);
void loadFromFlash  (AppConfig* config);
void _loadPanic     (AppConfig* config);

#endif