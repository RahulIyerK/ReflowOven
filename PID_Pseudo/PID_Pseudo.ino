void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

#define rTime 1

double total_error = 0;

void setTemp(uint16_t setPoint){
  //Set the endtime with a value relative to the phase of the profile it is in
  
  uint16_t currentTemp = 0;
  double error = 0, derivative = 0, correction = 0, prev_error = -999;
  
  //Find how to set Kp, Ki, Kd
  double Kp, Ki, Kd;
              
  currentTemp = thermocouple.readCelsius();

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

void setStep(double currentT, double nextT, double t) {
  double rate = (nextT - currentT)/rTime; //Degrees Celsius/s
  double tempSetPoint = currentT + rate;
  while (tempSetPoint < nextT) {
    setTemp(tempSetPoint, rTime);
    tempSetPoint = currentT;
  }
}


