/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include <unistd.h>
#include "chrono/ClockTime.hpp"
#include "chrono/Time.hpp"
#include "api/ChronoObject.hpp"

using namespace api;

void chrono::wait(const Microseconds & duration){
	Microseconds period = duration;
	u32 seconds = period.seconds();
	if( seconds > 0 ){
		::sleep(seconds);
		period = period - Seconds(seconds);
	}
	::usleep(period.microseconds());
}
