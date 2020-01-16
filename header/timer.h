#ifndef __TIMER__
#define __TIMER__
#include <time.h>
#include "const.h"

class Timer
{
	private:
		clock_t start_time  ;
		clock_t finish_time ;
		double time ;
		char time_string[BUFMAX_CHAR] ;

	public:
		Timer() ;
		void start() ;
		void stop() ;

		/* s=second, m=minute, h=hour */
		double get_s() ;
		double get_m() ;
		double get_h() ;
		char* display_time() ;
} ;

#endif
