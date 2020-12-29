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
  Config = new AppConfig;   // This will only get deleted on shutdown. No need to handle it gracefully.

  // Wake up the MIDI port
  MIDI.begin(Config->getChannel());
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

  if (data[0] != SYSEX_HEADER || data[1] != SYSEX_MFR) {
    digitalWrite(PIN_LEFT_EYE, HIGH);
    digitalWrite(PIN_RIGHT_EYE, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    return;
  } else {
    Config->setChannel(data[2]);
    Config->setCC(data[3]);
    Config->setValueMin(data[4]);
    Config->setValueMax(data[5]);
    Config->storeToFlash();
    
    // Wink before reset! ;)
    digitalWrite(PIN_LEFT_EYE, HIGH);
    digitalWrite(PIN_RIGHT_EYE, HIGH);
    delay(500);
    digitalWrite(PIN_LEFT_EYE, LOW);
    delay(500);
    digitalWrite(PIN_LEFT_EYE, HIGH);
    delay(500);
    resetFunc();
  }
}


void onRecvCC(byte channel, byte controller, byte val)
{
  // Do SPI stuff
}