#include "AppConfig.h"


AppConfig::AppConfig()
{
    // On init, restore from flash
    this->loadFromFlash();
}

void AppConfig::storeToFlash()
{
    // Serialize:
    int offset = 0;
    uint8_t serialArr[sizeof(AppConfig)+LEN_FLASH_OK_TOKEN];
    const char* header = FLASH_OK_TOKEN;

    memcpy(&serialArr+offset, &header, LEN_FLASH_OK_TOKEN);
    offset += LEN_FLASH_OK_TOKEN;

    memcpy(&serialArr+offset, &this->channel, sizeof(this->channel));
    offset += sizeof(this->channel);
    memcpy(&serialArr+offset, &this->cc, sizeof(this->cc));
    offset += sizeof(this->cc);
    memcpy(&serialArr+offset, &this->valueMin, sizeof(this->valueMin));
    offset += sizeof(this->valueMin);
    memcpy(&serialArr+offset, &this->valueMax, sizeof(this->valueMax));
    offset += sizeof(this->valueMax);

    // Write:
    for (int i = 0; i < offset; i++) {
        EEPROM.update(FLASH_OFFSET_APPCONFIG+i, serialArr[i]);
    }
}

void AppConfig::loadFromFlash()
{
    size_t length = sizeof(AppConfig)+LEN_FLASH_OK_TOKEN;
    int offset = 0;
    uint8_t serialArr[length];
    char headerBuf[LEN_FLASH_OK_TOKEN];

    // Read:
    //   Validate header:
    for (size_t i = 0; i < LEN_FLASH_OK_TOKEN; i++) {
        headerBuf[i] = EEPROM.read(i);
    }
    const char* header = (const char*)headerBuf;
    if (strcmp(header, FLASH_OK_TOKEN) != 0) {      // Shenanigans occurred. Panic.
        this->_loadPanic();
        return;
    }

    //   Else it's not all gone to fuck, read the rest:
    for (size_t i = LEN_FLASH_OK_TOKEN+0; i < LEN_FLASH_OK_TOKEN+length; i++) {
        serialArr[i] = EEPROM.read(i);
    }

    // Unpack:
    this->setChannel(serialArr[offset]);
    offset += sizeof(this->channel);
    this->setCC(serialArr[offset]);
    offset += sizeof(this->cc);
    this->setValueMin(serialArr[offset]);
    offset += sizeof(this->valueMin);
    this->setValueMax(serialArr[offset]);
    offset += sizeof(this->valueMax);
}

void AppConfig::_loadPanic()
{
    // Restore safe defaults:
    this->setChannel(DEFAULT_CHANNEL);
    this->setCC(DEFAULT_CC);
    this->setValueMin(DEFAULT_MIN);
    this->setValueMax(DEFAULT_MAX);

    // Wipe the EEPROM:
    for (int i = FLASH_OFFSET_APPCONFIG;
         i < FLASH_OFFSET_APPCONFIG + sizeof(AppConfig) + LEN_FLASH_OK_TOKEN;
         i++)
    {
        EEPROM.write(i, 0xFF);
    }

    // Leave it in kinda an ok state:
    this->storeToFlash();
}


#pragma region setters  //      ==      ==      ==      ==      ==

void AppConfig::setChannel(uint8_t val)
{
    if (val < MIDI_MAX)
        this->channel = val;
}

void AppConfig::setCC(uint8_t val)
{
    if (val < MIDI_MAX)
        this->cc = val;
}

void AppConfig::setValueMin(uint8_t val)
{
    if (val < MIDI_MAX)
        this->valueMin = val;
}

void AppConfig::setValueMax(uint8_t val)
{
    if (val < MIDI_MAX)
        this->valueMax = val;
}


#pragma endregion
#pragma region getters  //      ==      ==      ==      ==      ==

uint8_t AppConfig::getChannel()
{
    return this->channel;
}

uint8_t AppConfig::getCC()
{
    return this->cc;
}

uint8_t AppConfig::getValueMin()
{
    return this->valueMin;
}

uint8_t AppConfig::getValueMax()
{
    return this->valueMax;
}


#pragma endregion