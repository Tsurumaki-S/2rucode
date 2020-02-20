#include <stdio.h>
#include "../header/timer.h"

Timer::Timer()
{
	start_time = clock() ;
}

void Timer::start()
{
	start_time = clock() ;
	return ;
}

void Timer::stop()
{
	finish_time = clock() ;
	time = static_cast<double>(finish_time-start_time) / CLOCKS_PER_SEC ;
	return ;
}

/* second */
double Timer::get_s()
{
	return time ;
}

/* minute */
double Timer::get_m()
{
	return time/60.0 ;
}

/* hour */
double Timer::get_h()
{
	return time/3600.0 ;
}

char* Timer::display_time()
{
	sprintf(time_string,"time: %f [sec] ( %f [min], %f [hour] )\n",get_s(),get_m(),get_h() ) ;
	return time_string ;
}

