#include <Ticker.h> //library for the timed software interrupt that we use for our custom PWM

#define incrementSize
#define temperature
double setpointemp, currenttemp, t;
int steps, phase = 0, counter = 0;
//test comment
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

void setup() {
    // put your setup code here, to run once:
    //Serial. //test
     //   currenttemp = 0;/*Room Temperature*/;
    //setpointtemp = /*Soak Temperature*/;
   // t = /*Relative to whatever we have -> (incrementSize*steps)*/;
 //   steps = (setpointtemp - currenttemp)/t;

 ticker.attach_ms(TICK_LENGTH, tick);
}

void loop() {
    // put your main code here, to run repeatedly:

//    if (counter == 0)
//    {
//        //SerialUSB.println("starting loop");
//    }
//
//    if (counter%1 == 0)
//    {
//        currenttemp = getTemp();
//    }
//
//    if (counter%3 == 0)
//    {
//        PID(phase);
//        ++phase;
//    }
//
//    if (counter%1 == 0)
//    {
//        //checkMinMax();
//    }
//
//    if (counter%3 == 0)
//    {
//        //updateScreen();?
//    }
//
//    if (counter%1 == 0)
//    {
//        //updateMinMax();
//    }
//
//    if (counter%10 == 0)
//    {
//        //checkStability();
//    }
//
//    ++counter;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t getTemp() {
    //Do conversions from input value
    return currenttemp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Useful Functions
/////////////////////////////////////////////////////////////////////////////////////////////////



