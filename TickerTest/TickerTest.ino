#include <Ticker.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define LED1 4


#define MAXCS   2
Adafruit_MAX31855 thermocouple(MAXCS);

int TICK_LENGTH = 20; //milliseconds

uint8_t dutyCycle = 10; //percentage (min 0, max 100)

Ticker ticker;

int counter = 0;

void tick() //this function is called every TICK_LENGTH milliseconds
{
  if (counter == 255) //reached end of a period, so we reset everything
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
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc
  
  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500); 
  
  
  pinMode(LED1, OUTPUT);

  //Serial.println("hi");
  ticker.attach_ms(TICK_LENGTH, tick);
}

void loop()
{
// basic readout test, just print the current temp
 Serial.print("Internal Temp = ");
 Serial.println(thermocouple.readInternal());
 Serial.println(millis());

 double c = thermocouple.readCelsius();
 if (isnan(c)) {
   Serial.println("Something wrong with thermocouple!");
 } else {
   Serial.print("C = "); 
  Serial.println(c);
}
 Serial.print("F = ");
 Serial.println(thermocouple.readFarenheit());

 delay(1000);
}









