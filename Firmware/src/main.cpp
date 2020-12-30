#include <Arduino.h>
#include <MIDI.h>
#include "AppConfig.h"

static AppConfig* Config;
MIDI_CREATE_DEFAULT_INSTANCE();

void onRecvSysex(byte* data, unsigned size);
void onRecvCC(byte channel, byte controller, byte val);
void(* resetFunc) (void) = 0;

void setup()
{
  // Init hardware:
  pinMode(LED_BUILTIN,    OUTPUT);
  pinMode(PIN_LEFT_EYE,   OUTPUT);
  pinMode(PIN_RIGHT_EYE,  OUTPUT);
  pinMode(PIN_SPI_SS,     OUTPUT);
  pinMode(PIN_SPI_MOSI,   OUTPUT);
  pinMode(PIN_SPI_SCK,    OUTPUT);

  // Load config:
  Config = new AppConfig;
  loadFromFlash(Config);

  // Wake up the MIDI port
  MIDI.begin(Config->channel);
  MIDI.setHandleSystemExclusive(onRecvSysex);
  MIDI.setHandleControlChange(onRecvCC);

  digitalWrite(PIN_LEFT_EYE, HIGH);
  digitalWrite(PIN_RIGHT_EYE, HIGH);
}


void loop()
{
  MIDI.read();
}


void onRecvSysex(byte* data, unsigned size)
{
  digitalWrite(PIN_LEFT_EYE, LOW);
  digitalWrite(PIN_RIGHT_EYE, LOW);
  digitalWrite(LED_BUILTIN, HIGH);

  if (data[0] != SYSEX_HEADER || data[1] != SYSEX_MFR || data[size] != SYSEX_FOOTER) {
    return;
  } else if (size = 5 && data[2] == 0xDE && data[3] == 0xAD) {  // Nuke flash
    for (int i = 0; i < EEPROM.length(); i++) {
      // loadFromFlash() will recreate safe values on reboot, this is probably redundant.
      EEPROM.write(i, 0xFF); 
    }
    resetFunc();

  } else {  // Probably a config message, lets hope for the best
    Config->channel = data[2];
    Config->cc = data[3];
    Config->valueMin = data[4];
    Config->valueMax = data[5];
    storeToFlash(Config);
    
    resetFunc();
  }
}


void onRecvCC(byte channel, byte controller, byte val)
{
  // Do SPI stuff
}