bool running = TRUE;

struct reflopoint
{
	//time is in seconds
	//temp is in Celsius
	int time;
	int temp;
}
struct refloprofile
{
	int total_time;
	//store reflow profile into an array of reflo points 
	reflopoint peaks[];
}

void process_peaks(refloprofile *foo, unsigned long time_start, unsigned long time_end)
{
	int next_peak_index = 1;
	int cur_time;
	int prev_time = 0;
	int target_temp = foo->peaks[next_peak_index].temp;
	int prev_temp= foo->peaks[next_peak_index-1].temp;
	int target_time = foo->peaks[next_peak_index].time;

	while (running){
		cur_time = millis() - time_start;
		//CODE TO GRAB CUR_TEMP FROM MICROCONTROLLER
		PID(cur_time, prev_time, cur_temp, (float)cur_time/target_time * (target_temp - prev_temp) + prev_temp); //PID implements change in temp
		if (cur_time >= target_time){
			++next_peak_index;
			target_temp = foo->peaks[next_peak_index].temp;
			prev_temp= foo->peaks[next_peak_index-1].temp;
			target_time = foo->peaks[next_peak_index].time;			
			time_start = millis();
			prev_time = 0;
			continue;
		}
		prev_time = cur_time;
		if (millis() > time_end)
			break; //can add other ways of exiting loop
	}
}

void sample()
{
	int num_peaks = 5;
	struct refloprofile *foo = malloc(offsetof(struct refloprofile, peaks) + sizeof(int) * num_peaks);

	//initial
	foo->peaks[0].time = 0;
	foo->peaks[0].temp = 0;

	//preheat or soaking
	foo->peaks[1].time = 80;
	foo->peaks[1].temp = 160;

	//heat
	foo->peaks[2].time = 150;
	foo->peaks[2].temp = 180;

	//re-flow
	foo->peaks[3].time = 60;
	foo->peaks[3].temp = 260;

	//cooling
	foo->peaks[3].time = 20;
	foo->peaks[3].temp = 260;

	//final
	foo->peaks[4].time = 80;
	foo->peaks[4].temp = 180;
	foo->total_time = 360

	int begin = millis();
	process_peaks(foo, begin, begin + 1000 * foo->total_time);
}

int main()
{
	sample();
}