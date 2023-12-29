#define ANALOG_IN_ESP32 34
#define PIEZO_POWER_ESP32 32

#define ANALOG_IN_RP2040 15
#define PIEZO_POWER_RP2040 16

#define ESP32 1
#define RP2040 2

#define POWER 1
#define PIN 2

bool note = false;
unsigned long initialms = 0;
unsigned long currms = 0;
int currMax = 0;
int board = RP2040;

int curr_power_pin()
{
  if (board == ESP32)
  {
    return ANALOG_IN_ESP32;
  }
  else
  {
    return ANALOG_IN_RP2040;
  }
}

int curr_input_pin()
{
  if (board == ESP32)
  {
    return PIEZO_POWER_ESP32;
  }
  else
  {
    return PIEZO_POWER_RP2040;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(curr_power_pin(), OUTPUT);
  digitalWrite(curr_power_pin(), HIGH);
}

void loop()
{
  // read the input on analog pin:
  int sensorValue = analogRead(curr_input_pin());
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
