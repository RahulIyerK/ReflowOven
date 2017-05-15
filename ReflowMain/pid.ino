bool running = TRUE;

struct reflowPoint
{
  //time is in seconds
  //temp is in Celsius
  int time;
  int temp;
};
struct reflowProfile
{
  uint8_t total_time;
  uint8_t m_numPoints;
  uint8_t m_nextPeak = 0;
  //store reflow profile into an array of reflo points
  reflowPoint peaks[];
};

void process_peaks(reflowProfile *profile)
{
  (profile->m_nextPeak)++;
  unsigned long cur_time;
  unsigned long start_time = millis();
  int target_temp = profile->peaks[profile->m_nextPeak].temp;
  // read in room temp, average value
  uint32_t ovenTemp = 0;
  for (int i = 0; i < 10; i++) {
    ovenTemp += getTemp();
  }
  ovenTemp /= 10;
  uint16_t prev_temp = (uint16_t)ovenTemp;
  unsigned long target_time = profile->peaks[profile->m_nextPeak].time * 1000;

  while (true) {
    runCurStep(target_temp, prev_temp, target_time, start_time);
    ++(profile->m_nextPeak);
    if (profile->m_nextPeak >= profile->m_numPoints)
      break;
    target_temp = profile->peaks[profile->m_nextPeak].temp;
    prev_temp = profile->peaks[profile->m_nextPeak - 1].temp;
    target_time = profile->peaks[profile->m_nextPeak].time * 1000;
    start_time = millis();

  }
}
bool runCurStep(uint16_t target_temp, uint16_t prev_temp, unsigned long target_time, unsigned long start_time) {
  unsigned long cur_time = millis() - start_time;
  float slope = (float)(target_temp - prev_temp) / target_time;
  while (cur_time < target_time) {
    cur_time = millis() - start_time;
    PID(prev_temp + slope * cur_time, getTemp()); //PID implements change in temp, analogWrites
  }
}
void setUpProfile(uint16_t* temps, uint8_t num_peaks, uint16_t* times)
{
  reflowProfile *profile = malloc(offsetof(struct reflowProfile, peaks) + sizeof(int) * num_peaks);
  profile->m_numSteps = num_peaks;

  //initial
  for (int i = 0; i < num_peaks; i++) {
    profile->peaks[i].time = times[i];
    profile->peaks[i].temp = temps[i];
  }
  process_peaks(profile);
}
