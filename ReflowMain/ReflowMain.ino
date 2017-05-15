#define incrementSize
#define temperature
double setpointemp, currenttemp, t;
int steps, phase = 0, counter = 0;
//test comment
/////////////////////////////////////////////////////////////////////////////////////////////////
// Main Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    // put your setup code here, to run once:
    //Serial. //test
     //   currenttemp = 0;/*Room Temperature*/;
    //setpointtemp = /*Soak Temperature*/;
   // t = /*Relative to whatever we have -> (incrementSize*steps)*/;
 //   steps = (setpointtemp - currenttemp)/t;
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

double total_error = 0;

void PID(uint16_t setPoint,uint16_t currentTemp){
  //Set the endtime with a value relative to the phase of the profile it is in
  
  double error = 0, derivative = 0, correction = 0, prev_error = -999;
  
  //Find how to set Kp, Ki, Kd
  double Kp, Ki, Kd;
            
  //PID
  error = setPoint - currentTemp;
  total_error += error;
  if(prev_error != -999)
    derivative = error - prev_error;
  correction = Kp*error + Ki*total_error + Kd*derivative;
  prev_error = error;

  //Change temperature based on PID
  currentTemp += correction;
    
  //Write to PWM

}



