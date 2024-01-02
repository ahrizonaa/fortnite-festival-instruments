#include <strings.h>
#include <Arduino.h>
#include <BleGamepad.h>

/*
- all times in ms
*/

BleGamepad bleGamepad("Smart Drums", "Mark Tech", 100);

const uint8_t PIEZO_THRESHOLD = 150;
// const uint16_t PIEZO_THRESHOLD_LG = 256;

const uint8_t NOTE_INTERVAL = 250;

const uint8_t XS = 0;
const uint8_t SM = 1;
const uint8_t MD = 2;
const uint8_t LG = 3;

const uint8_t PIN_IN_XS = 13;
const uint8_t PIN_IN_SM = 12;
const uint8_t PIN_IN_MD = 14;
const uint8_t PIN_IN_LG = 27;

const uint8_t PIN_OUT_XS = 26;
const uint8_t PIN_OUT_SM = 25;
const uint8_t PIN_OUT_MD = 33;
const uint8_t PIN_OUT_LG = 32;

const String X = "XS";
const String S = "SM";
const String M = "MD";
const String L = "LG";

// CONFIG DRUMS IN USE
const uint8_t DRUMSET[4] = {XS, SM, MD, LG};

bool activeNotes[4] = {false, false, false, false};
unsigned long startTimes[4] = {0, 0, 0, 0};
uint16_t signals[4] = {0, 0, 0, 0};
unsigned long currTime = 0;

uint8_t devicein(uint8_t drum)
{
  uint8_t pin;
  pin = drum == XS ? PIN_IN_XS : drum == SM ? PIN_IN_SM
                             : drum == MD   ? PIN_IN_MD
                                            : PIN_IN_LG;
  return pin;
}

uint8_t deviceout(uint8_t drum)
{
  uint8_t pin;
  pin = drum == XS ? PIN_OUT_XS : drum == SM ? PIN_OUT_SM
                              : drum == MD   ? PIN_OUT_MD
                                             : PIN_OUT_LG;
  return pin;
}

String devicename(uint8_t drum)
{
  return drum == XS ? X : drum == SM ? S
                      : drum == MD   ? M
                                     : L;
}

void ReadDrum(uint8_t drum)
{
  if (activeNotes[drum] == false)
  {
    uint16_t val = analogRead(devicein(drum));

    if (val >= PIEZO_THRESHOLD)
    {
      signals[drum] = val;
      activeNotes[drum] = true;
      startTimes[drum] = millis();
      if (bleGamepad.isConnected())
      {
        bleGamepad.press(BUTTON_1);
        bleGamepad.release(BUTTON_1);
      }
    }
  }
  else if (activeNotes[drum] == true)
  {
    currTime = millis();
    if (currTime - startTimes[drum] > NOTE_INTERVAL)
    {
      /*
        note capture expired:
          1.  print captured note
          2.  reset note
      */
      if (signals[drum] >= PIEZO_THRESHOLD)
      {
        Serial.print(signals[drum]);
        Serial.println(" " + devicename(drum));
      }

      activeNotes[drum] = false;
      startTimes[drum] = 0;
      signals[drum] = 0;
    }
    else
    {
      // ignore residual signals during note capture interval, do nothing
    }
  }
}

void setup()
{
  Serial.begin(115200);
  bleGamepad.begin();
  for (uint8_t i = 0; i < sizeof(DRUMSET) / sizeof(DRUMSET[0]); i++)
  {
    pinMode(devicein(DRUMSET[i]), INPUT);
    pinMode(deviceout(DRUMSET[i]), OUTPUT);
    digitalWrite(deviceout(DRUMSET[i]), HIGH);
  }
}

void loop()
{
  for (uint8_t i = 0; i < sizeof(DRUMSET) / sizeof(DRUMSET[0]); i++)
  {
    ReadDrum(DRUMSET[i]);
  }
  // Serial.println();
  delay(10); // delay in between reads for stability
}
