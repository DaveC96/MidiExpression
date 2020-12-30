#include "AppConfig.h"

void storeToFlash(AppConfig* config)
{
    // Serialize:
    int offset = 0;
    AppConfig_serial serialArr[sizeof(AppConfig)+LEN_FLASH_OK_TOKEN];
    const char* header = FLASH_OK_TOKEN;

    memcpy(&serialArr+offset, &header, LEN_FLASH_OK_TOKEN);
    offset += LEN_FLASH_OK_TOKEN;

    memcpy(&serialArr+offset, &config->channel, sizeof(config->channel));
    offset += sizeof(config->channel);
    memcpy(&serialArr+offset, &config->cc, sizeof(config->cc));
    offset += sizeof(config->cc);
    memcpy(&serialArr+offset, &config->valueMin, sizeof(config->valueMin));
    offset += sizeof(config->valueMin);
    memcpy(&serialArr+offset, &config->valueMax, sizeof(config->valueMax));
    offset += sizeof(config->valueMax);

    // Write:
    for (int i = 0; i < offset; i++) {
        EEPROM.update(FLASH_OFFSET_APPCONFIG+i, serialArr[i]);
    }
}

void loadFromFlash(AppConfig* config)
{
    size_t length = sizeof(AppConfig)+LEN_FLASH_OK_TOKEN;
    int offset = 0;
    AppConfig_serial serialArr[length];
    char headerBuf[LEN_FLASH_OK_TOKEN];

    // Read:
    //   Validate header:
    for (size_t i = 0; i < LEN_FLASH_OK_TOKEN; i++) {
        headerBuf[i] = EEPROM.read(i);
    }
    const char* header = (const char*)headerBuf;
    if (strcmp(header, FLASH_OK_TOKEN) != 0) {      // Shenanigans occurred. Panic.
        _loadPanic(config);
        return;
    }

    //   Else it's not all gone to fuck, read the rest:
    for (size_t i = LEN_FLASH_OK_TOKEN+0; i < LEN_FLASH_OK_TOKEN+length; i++) {
        serialArr[i] = EEPROM.read(i);
    }

    // Unpack:
    config->channel = serialArr[offset];
    offset += sizeof(config->channel);
    config->cc = serialArr[offset];
    offset += sizeof(config->cc);
    config->valueMin = serialArr[offset];
    offset += sizeof(config->valueMin);
    config->valueMax = serialArr[offset];
    offset += sizeof(config->valueMax);
}

void _loadPanic(AppConfig* config)
{
    // Restore safe defaults:
    config->channel = DEFAULT_CHANNEL;
    config->cc = DEFAULT_CC;
    config->valueMin = DEFAULT_MIN;
    config->valueMax = DEFAULT_MAX;

    // Wipe the EEPROM:
    for (int i = FLASH_OFFSET_APPCONFIG;
         i < FLASH_OFFSET_APPCONFIG + sizeof(AppConfig) + LEN_FLASH_OK_TOKEN;
         i++)
    {
        EEPROM.write(i, 0xFF);
    }

    // Leave it in kinda an ok state:
    storeToFlash(config);
}