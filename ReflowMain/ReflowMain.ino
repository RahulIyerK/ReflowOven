#define incrementSize
#define temperature
double setpointemp, currenttemp, t;
int steps, phases;
//GITHUB yo
void setup() {
  // put your setup code here, to run once:
  Serial. //test
  currenttemp = /*Room Temperature*/;
  setpointtemp = /*Soak Temperature*/;
  t = /*Relative to whatever we have -> (incrementSize*steps)*/;
  steps = (setpointtemp - currenttemp)/t;
  phase = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  currenttemp = getTemp();
  if (getNextStep(currenttemp, setpointtemp) && phase != 2) {
    setStep(currenttemp, setpointtemp, millis());
    phase++;
  }
  steps++;

  switch(phase) {
    case 2: break;
    case 3: break;
    case 4: break;
    default: continue;
  }
}

double getTemp() {
  //Do conversions from input value
  return currenttemp;
}

void setStep(double currenttemp, double setpointtemp, double t) {
  return (setpointtemp - currenttemp)/t; //Returns the number of steps that will be used
}

bool getNextStep(double current, double set) {
  if (current == set)
    return true;
  return false;  
}

