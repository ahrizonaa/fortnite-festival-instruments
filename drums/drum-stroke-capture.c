#include <strings.h>

/*
- all times in ms
*/

const uint8_t PIEZO_THRESHOLD = 150;
// const uint16_t PIEZO_THRESHOLD_LG = 256;

const uint8_t XS = 0;
const uint8_t SM = 1;
const uint8_t MD = 2;
const uint8_t LG = 3;

const uint8_t PIN_IN_XS = 32;
const uint8_t PIN_IN_SM = 33;
const uint8_t PIN_IN_MD = 25;
const uint8_t PIN_IN_LG = 26;

const uint8_t PIN_OUT_XS = 27;
const uint8_t PIN_OUT_SM = 14;
const uint8_t PIN_OUT_MD = 12;
const uint8_t PIN_OUT_LG = 13;

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
  pin = drum == 0 ? PIN_IN_XS : drum == 1 ? PIN_IN_SM
                            : drum == 2   ? PIN_IN_MD
                                          : PIN_IN_LG;
  return pin;
}

uint8_t deviceout(uint8_t drum)
{
  uint8_t pin;
  pin = drum == 0 ? PIN_OUT_XS : drum == 1 ? PIN_OUT_SM
                             : drum == 2   ? PIN_OUT_MD
                                           : PIN_OUT_LG;
  return pin;
}

String devicename(uint8_t drum)
{
  return drum == 0 ? X : drum == 1 ? S
                     : drum == 2   ? M
                                   : L;
}

void ReadDrum(uint8_t drum)
{
  if (activeNotes[drum] == false)
  {
    uint16_t val = analogRead(devicein(drum));

    if (val >= PIEZO_THRESHOLD)
    {
      // signals[drum] = max(signals[drum], val);
      signals[drum] = val;
      activeNotes[drum] = true;
      startTimes[drum] = millis();
    }
  }
  else if (activeNotes[drum] == true)
  {
    currTime = millis();
    if (currTime - startTimes[drum] > 200)
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
  Serial.begin(9600);

  for (uint8_t i = 0; i < sizeof(DRUMSET) / sizeof(DRUMSET[0]); i++)
  {
    pinMode(deviceout(DRUMSET[i]), OUTPUT);
    pinMode(devicein(DRUMSET[i]), INPUT);
    digitalWrite(deviceout(DRUMSET[i]), HIGH);
  }
}

void loop()
{
  for (uint8_t i = 0; i < sizeof(DRUMSET) / sizeof(DRUMSET[0]); i++)
  {
    ReadDrum(DRUMSET[i]);
  }
  Serial.println();
  delay(100); // delay in between reads for stability
}
