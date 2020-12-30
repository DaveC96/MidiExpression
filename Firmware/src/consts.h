#ifndef CONSTS_H
#define CONSTS_H

// This is just some arbitrary byte pattern we can use to know that the flash is valid.
// In this case, it's probably longer than the actual data. Oh well.
// ATmega128 has 512 whole bytes of EEPROM, so we should be ok.
#define FLASH_OK_TOKEN      (char*)"Elephant"
#define LEN_FLASH_OK_TOKEN  8

// EEPROM address for the app config to be stored to / recalled from
#define FLASH_OFFSET_APPCONFIG 0

// Used to validate arguments for AppConfig setters
#define MIDI_MAX        127
#define SYSEX_HEADER    0xF0
#define SYSEX_FOOTER    0xF7
#define SYSEX_MFR       0x00

// Safe defaults for AppConfig::_loadPanic() when FLASH_OK_TOKEN turns out to be uncharacteristically not ok
#define DEFAULT_CHANNEL 0           // MIDI_CHANNEL_OMNI -- but I don't really want to include MIDI.h in here...
#define DEFAULT_CC      4           // "Foot controller"
#define DEFAULT_MIN     0
#define DEFAULT_MAX     MIDI_MAX

// HARDWARE ================================================
#define PIN_LEFT_EYE    6           // D3
#define PIN_RIGHT_EYE   7           // D4

#define SPI_DELAY_MS    100         // ...Long enough to pass the edge

#endif