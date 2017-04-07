#include <Ticker.h>

#define LED1 12            

#define PWM_PERIOD 1000 //milliseconds

double dutyCycle = .75;

unsigned long onTime = dutyCycle * PWM_PERIOD;

Ticker flasher;



void flash() //this function is called every PWM_PERIOD milliseconds
{
  wdt_disable();
  unsigned long startMillis = millis();
  unsigned long endMillis = startMillis + PWM_PERIOD;
  
  Serial.print(startMillis);
  Serial.print(" ");
  Serial.println(onTime);
  
  unsigned long curMillis = startMillis;
  unsigned long stopMillis =  startMillis + onTime;

  while (curMillis < endMillis)
  {
      if (curMillis <= stopMillis)
      {
        Serial.println("writing HIGH");
        digitalWrite(LED1, HIGH);
      }
      else
      {
        Serial.println("writing LOW");
        digitalWrite(LED1, LOW);
      }
      
      curMillis++;
  }
  wdt_enable(WDTO_2S);
}

void setup() 
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  
  digitalWrite(LED1, LOW);
  flasher.attach_ms(PWM_PERIOD, flash);
  
}

void loop() 
{
//  digitalWrite(LED1, HIGH);
//  delay (250);
//  digitalWrite(LED1, LOW);
//  delay (250);
}
