//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "chrono/Timer.hpp"
#include "chrono/Clock.hpp"
using namespace chrono;


Timer::Timer() { reset(); }

void Timer::reset(){
	m_start.reset();
	m_stop.reset();
	//when stop.seconds() value is 0, the timer is in reset mode
	//when stop.seconds() value is -1, the timer is currently running
}

void Timer::restart(){
	m_start = Clock::get_time();
	m_stop.set(Seconds::invalid(), Nanoseconds(0));
}


void Timer::start(){
	if( is_running() == false ){
		restart();
	}
}

void Timer::resume(){
	ClockTime new_start;
	ClockTime now;

	if( m_stop.seconds() < 0 ){
		return; //timer is not stopped
	}

	//if timer has been stopped, then resume counting
	if( m_start.seconds() + m_start.nanoseconds() ){ //start is non-zero
		new_start = m_stop - m_start;
		now = Clock::get_time();
		m_start = now - new_start;
		m_stop.set(Seconds::invalid(), Nanoseconds(0));
	} else {
		//if timer is not running then start it
		restart();
	}
}

ClockTime Timer::clock_time() const {
	ClockTime now;
	if( m_stop.seconds() < 0 ){
		now = Clock::get_time();
	} else {
		now = m_stop;
	}
	//difference between now and start_
	now -= m_start;
	return now;
}

Microseconds Timer::calc_value() const {
	return Microseconds(clock_time());
}

void Timer::stop(){
	if( is_running() ){
		m_stop = Clock::get_time();
	}
}

