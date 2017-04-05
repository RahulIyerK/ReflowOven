#include <Ticker.h>

#define LED1 1
#define LED2 2
#define TICK_SECONDS 0.3

Ticker flipper;

int count = 0;

void flip()
{
  bool state = digitalRead(lED1);  // get the current state of GPIO1 pin
  digitalWrite(LED1, !state);     // set pin to the opposite state
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  
  // flip the pin every 0.3s
  flipper.attach(TICK_SECONDS, flip);
}

void loop() {
  digitalWrite (LED2, HIGH);
  delay (1000);
  digitalWrite (LED2, LOW);
  delay (1000);
}
