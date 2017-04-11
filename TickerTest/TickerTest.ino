#include <Ticker.h>

#define LED1 12            

#define PWM_PERIOD 50 //milliseconds

double dutyCycle = 75; //percentage (min 0, max 100)
bool signal_state = HIGH;

Ticker ticker;

int counter = 0; 

void tick() //this function is called every PWM_PERIOD milliseconds
{
//  wdt_disable();
//  unsigned long startMillis = millis();
//  unsigned long endMillis = startMillis + PWM_PERIOD;
//  
//  Serial.print(startMillis);
//  Serial.print(" ");
//  Serial.println(onTime);
//  
//  unsigned long curMillis = startMillis;
//  unsigned long stopMillis =  startMillis + onTime;
//
//  while (curMillis < endMillis)
//  {
//      if (curMillis <= stopMillis)
//      {
//        Serial.println("writing HIGH");
//        digitalWrite(LED1, HIGH);
//      }
//      else
//      {
//        Serial.println("writing LOW");
//        digitalWrite(LED1, LOW);
//      }
//      
//      curMillis++;
//  }
//  wdt_enable(WDTO_2S);

  digitalWrite(LED1, signal_state);


  counter++;

  Serial.println(counter);
  if (counter == dutyCycle) //reached end of duty cycle, so we set state to LOW (it was HIGH until now)
  {
    signal_state = LOW;
  }
  
  if (counter == 100) //reached end of a period, so we reset everything
  {
    signal_state = HIGH;
    counter = 0;
  }
}

void setup() 
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  
  ticker.attach_ms(PWM_PERIOD, tick);
  
}

void loop() 
{
//  digitalWrite(LED1, HIGH);
//  delay (250);
//  digitalWrite(LED1, LOW);
//  delay (250);
}
