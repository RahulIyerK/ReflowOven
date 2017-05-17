#include <Ticker.h> //library for the timed software interrupt that we use for our custom PWM
#define incrementSize
#define temperature

#include <SPI.h>
#include "Adafruit_MAX31855.h"
#define MAXCS   2
Adafruit_MAX31855 thermocouple(MAXCS);

double setpointemp, currenttemp, t;
int steps, phase = 0, counter = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////
// PWM Ticker
/////////////////////////////////////////////////////////////////////////////////////////////////

#define TICK_LENGTH 20 //tick length in milliseconds
#define FULL_DUTY_VAL 100 //max duty cycle value allowed

#define relay 2 //digital pin for switching relay

uint8_t pwm_dutyCycle = 0; //percentage (min 0, max 100DUTY_VAL)

Ticker ticker;

int pwm_counter = 0;

void tick() //this function is called every TICK_LENGTH milliseconds
{
  if (pwm_counter >= FULL_DUTY_VAL) //reached end of a period, so we reset everything
  {
    pwm_counter = 0;
  }

  digitalWrite(relay, (pwm_counter < pwm_dutyCycle));

  pwm_counter++;

  //Serial.println(pwm_counter);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t getTemp() {
  //returns temperature in 1/4 Celsius)
  return thermocouple.readCelsius();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Useful Functions
/////////////////////////////////////////////////////////////////////////////////////////////////



