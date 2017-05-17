#define Kp 1.0
#define Ki 1.0
#define Kd 1.0

double total_error = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structures used to store reflow profile
////////////////////////////////////////////////////////////////////////////////////////////////////////
struct reflowPoint
{
  int time; //time between steps (seconds)
  int temp; //target temperature for step (1/4 Celsius)
};
struct reflowProfile
{
  uint8_t total_time;
  uint8_t m_numPoints;
  uint8_t m_nextPeak = 0;
  //store reflow profile into an array of reflo points
  reflowPoint peaks[];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Useful Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////

void process_peaks(struct reflowProfile *profile)
{
  
  (profile->m_nextPeak)++;
  unsigned long cur_time;
  unsigned long start_time = millis();
  int target_temp = profile->peaks[profile->m_nextPeak].temp;
  unsigned long target_time = profile->peaks[profile->m_nextPeak].time * 1000;
  
  // read in room temp, average value
  uint32_t ovenTemp = 0;
  for (int i = 0; i < 10; i++) {
    ovenTemp += getTemp();
  }
  ovenTemp /= 10;
  uint32_t prev_temp = (uint32_t)ovenTemp;

  while (true) {
    // Follow path for current step
    runCurStep(target_temp, prev_temp, target_time, start_time);

    //Get ready for next step
    ++(profile->m_nextPeak);
    if (profile->m_nextPeak >= profile->m_numPoints)
      break;

    target_temp = profile->peaks[profile->m_nextPeak].temp;
    prev_temp = profile->peaks[profile->m_nextPeak - 1].temp;
    target_time = profile->peaks[profile->m_nextPeak].time * 1000;
    start_time = millis();
  }
}

bool runCurStep(uint32_t target_temp, uint32_t prev_temp, unsigned long target_time, unsigned long start_time) 
{
  unsigned long cur_time = millis() - start_time;
  float slope = (float)(target_temp - prev_temp) / target_time;
  while (cur_time < target_time) {
    cur_time = millis() - start_time;
    PID(prev_temp + slope * cur_time, getTemp()); //PID implements change in temp, analogWrites
  }
}

void setUpProfile(uint32_t* temps, uint8_t num_peaks, uint32_t* times)
{
  //dynamically allocate enough space for reflopoints
  reflowProfile *profile = (reflowProfile*)(malloc(offsetof(struct reflowProfile, peaks) + sizeof(int) * num_peaks));
  profile->m_numPoints = num_peaks;

  //creates the profile
  for (int i = 0; i < num_peaks; i++) {
    profile->peaks[i].time = times[i];
    profile->peaks[i].temp = temps[i]<<2;
  }
  process_peaks(profile);
}

void PID(uint32_t setPoint,uint32_t currentTemp)
{  
  double error = 0, derivative = 0, correction = 0, prev_error = -999; //fix
            
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

