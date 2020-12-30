#include <Arduino.h>
#include <MIDI.h>
#include "AppConfig.h"
#include "mcp4131.h"

static AppConfig* Config;
static mcp4131* DigitalPot;
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

  SPI.begin();
  DigitalPot = new mcp4131(PIN_SPI_SS, PIN_SPI_MOSI, PIN_SPI_SCK);

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


void onRecvSysex(byte* data, unsigned int size)
{
  digitalWrite(PIN_LEFT_EYE, LOW);
  digitalWrite(PIN_RIGHT_EYE, LOW);
  digitalWrite(LED_BUILTIN, HIGH);

  if (data[0] != SYSEX_HEADER || data[1] != SYSEX_MFR || data[size] != SYSEX_FOOTER) {
    return;
  } else if ((size = 5) && (data[2] == 0xDE) && (data[3] == 0xAD)) {  // Nuke flash
    for (unsigned int i = 0; i < EEPROM.length(); i++) {
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
  if (channel == Config->channel && controller == Config->cc) {
    uint8_t pwm = val*2;
    analogWrite(PIN_LEFT_EYE, pwm);
    analogWrite(PIN_RIGHT_EYE, pwm);
    DigitalPot->writeValue(val);
  }
}