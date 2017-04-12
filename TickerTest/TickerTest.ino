#include <Ticker.h>

#define LED1 12

int TICK_LENGTH = 15; //milliseconds

double dutyCycle = 50; //percentage (min 0, max 100)

Ticker ticker;

int counter = 0;

void tick() //this function is called every TICK_LENGTH milliseconds
{
  if (counter == 100) //reached end of a period, so we reset everything
  {
    counter = 0;
  }
  
  digitalWrite(LED1, (counter < dutyCycle));
  
  counter++;

  Serial.println(counter);  
}

void setup()
{
  
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);

  Serial.println("hi");
  ticker.attach_ms(TICK_LENGTH, tick);
}

void loop()
{
  //wat
}
