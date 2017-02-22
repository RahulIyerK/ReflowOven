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
    Serial. //test
        currenttemp = /*Room Temperature*/;
    setpointtemp = /*Soak Temperature*/;
    t = /*Relative to whatever we have -> (incrementSize*steps)*/;
    steps = (setpointtemp - currenttemp)/t;
}

void loop() {
    // put your main code here, to run repeatedly:

    if (counter == 0)
    {
        //SerialUSB.println("starting loop");
    }

    if (counter%1 == 0)
    {
        currenttemp = getTemp();
    }

    if (counter%3 == 0)
    {
        PID(phase);
        ++phase;
    }

    if (counter%1 == 0)
    {
        //checkMinMax();
    }

    if (counter%3 == 0)
    {
        //updateScreen();?
    }

    if (counter%1 == 0)
    {
        //updateMinMax();
    }

    if (counter%10 == 0)
    {
        //checkStability();
    }

    ++counter;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////
// Useful Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

void PID()
{
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
