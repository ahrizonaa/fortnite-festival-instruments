#include <stdio.h>

enum DRUMS
{
  XS = 0,
  SM = 1,
  MD = 2,
  LG = 3,
  IN_XS = 12,
  IN_SM = 13,
  IN_MD = 14,
  IN_LG = 15,
  OUT_XS = 36,
  OUT_SM = 39,
  OUT_MD = 41,
  OUT_LG = 40
};

const char X[] = "XS";
const char S[] = "SM";
const char M[] = "MD";
const char L[] = "LG";

bool notes[4];
unsigned long startms[4];
int vals[4];
unsigned long currms = 0;

int devicein(int drum)
{
  return drum == 0 ? IN_XS : drum == 1 ? IN_SM
                         : drum == 2   ? IN_MD
                                       : IN_LG;
}

int deviceout(int drum)
{
  return drum == 0 ? OUT_XS : drum == 1 ? OUT_SM
                          : drum == 2   ? OUT_MD
                                        : OUT_LG;
}

String devicename(int drum)
{
  return drum == 0 ? X : drum == 1 ? S
                     : drum == 2   ? M
                                   : L;
}

void ReadDrum(int drum)
{
  if (notes[drum] == false)
  {
    vals[drum] = max(vals[drum], analogRead(devicein(drum)));
    notes[drum] = true;
    startms[drum] = millis();
  }
  else
  {
    currms = millis();
    if (currms - startms[drum] > 200)
    {
      // note captured
      notes[drum] = false;
      startms[drum] = 0;
      Serial.print(vals[drum]);
      Serial.println(" " + devicename(drum));
      vals[drum] = 0;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(OUT_XS, OUTPUT);
  pinMode(OUT_SM, OUTPUT);
  pinMode(OUT_MD, OUTPUT);
  pinMode(OUT_LG, OUTPUT);

  digitalWrite(OUT_XS, HIGH);
  digitalWrite(OUT_SM, HIGH);
  digitalWrite(OUT_MD, HIGH);
  digitalWrite(OUT_LG, HIGH);
}

void loop()
{
  ReadDrum(XS);
  ReadDrum(SM);
  ReadDrum(MD);
  ReadDrum(LG);
  delay(10); // delay in between reads for stability
}
