bool note = false;
unsigned long initialms = 0;
unsigned long currms = 0;
int currMax = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // read the input on analog pin 34:
  int sensorValue = analogRead(34);
  // print out the value you read:
  if (sensorValue >= 100)
  {
    if (note == false)
    {
      note = true;
      currMax = sensorValue;
      initialms = millis();
    }
    else
    {
      currms = millis();
      if (currms - initialms < 200)
      {
        if (sensorValue > currMax)
        {
          currMax = sensorValue;
        }
      }
      else
      {
        // print note captured
        Serial.println(currMax);
        // reset all
        note = false;
        currMax = 0;
        initialms = 0;
        currms = 0;
      }
    }
  }
  delay(10); // delay in between reads for stability
}