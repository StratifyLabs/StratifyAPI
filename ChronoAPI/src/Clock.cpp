/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ChronoAPI/Clock.hpp"

#if defined __macosx
#include <sys/time.h>
static int clock_gettime(int clk_id, struct timespec* t) {
	MCU_UNUSED_ARGUMENT(clk_id);
	struct timeval now;
	int rv = gettimeofday(&now, NULL);
	if (rv) return rv;
	t->tv_sec  = now.tv_sec;
	t->tv_nsec = now.tv_usec * 1000;
	return 0;
}
#endif

using namespace chrono;

ClockTime Clock::get_time(enum chrono::Clock::clock_id  clock_id){
	ClockTime clock_time;
	if( clock_gettime(clock_id, clock_time) < 0 ){
		clock_time = ClockTime::invalid();
	}
	return clock_time;
}


ClockTime Clock::get_resolution(enum chrono::Clock::clock_id clock_id){
#if defined __macosx

	ClockTime resolution(Seconds(0), Nanoseconds(1000));

#else
	ClockTime resolution;
	if( clock_getres(clock_id, resolution) < 0 ){
		resolution = ClockTime::invalid();
	}
#endif
	return resolution;
}
