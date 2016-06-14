//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <unistd.h>
#include <time.h>
#include "sys/Timer.hpp"
using namespace sys;


void Timer::wait_sec(u32 timeout){
	sleep(timeout);
}

void Timer::wait_msec(u32 timeout){
	if( (timeout * 1000) < 1000000 ){
		usleep(timeout*1000);
	} else {
		for(u32 i = 0; i < timeout; i++){
			usleep(1000);
		}
	}
}

void Timer::wait_usec(u32 timeout){
	usleep(timeout);
}

#if !defined __link

static struct timespec diff(struct timespec now, struct timespec then){
	struct timespec d;
	d.tv_nsec = now.tv_nsec - then.tv_nsec;
	d.tv_sec = now.tv_sec - then.tv_sec;
	if( d.tv_nsec < 0 ){
		d.tv_nsec += 1000000000;
		d.tv_sec -= 1;
	}
	return d;
}

Timer::Timer() { reset(); }

void Timer::reset(){
	m_start.tv_nsec = 0;
	m_start.tv_sec = 0;
	m_stop.tv_nsec = 0;

	//when this value is 0, the timer is in reset mode
	//when this value is -1, the timer is currently running
	m_stop.tv_sec = 0;
}


int Timer::get_clock_usec(){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec*1000000 + (now.tv_nsec) / 1000;
}

int Timer::get_clock_msec(){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec*1000 + (now.tv_nsec) / 1000000;
}

int Timer::get_clock_sec(){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec;
}

void Timer::restart(){
	clock_gettime(CLOCK_REALTIME, &m_start);
	m_stop.tv_sec = -1;
}


void Timer::start(){
	if( is_running() == false ){
		restart();
	}
}

void Timer::resume(){
	struct timespec new_start;
	struct timespec now;

	if( m_stop.tv_sec < 0 ){
		return; //timer is not stopped
	}

	//if timer has been stopped, then resume counting
	if( m_start.tv_sec + m_start.tv_nsec ){
		new_start = diff(m_stop, m_start);
		clock_gettime(CLOCK_REALTIME, &now);
		m_start = diff(now, new_start);
		m_stop.tv_sec = -1;
	} else {
		//if timer is not running then start it
		restart();
	}
}

u32 Timer::calc_sec() const {
	struct timespec now;

	if( m_stop.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = m_stop;
	}
	//difference between now and start time
	return now.tv_sec - m_start.tv_sec;
}

u32 Timer::calc_msec() const {
	struct timespec now;
	if( m_stop.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = m_stop;
	}
	//difference between now and start_
	now = diff(now, m_start);
	return now.tv_sec*1000 + (now.tv_nsec + 500000) / 1000000;
}

u32 Timer::calc_usec() const {
	struct timespec now;
	if( m_stop.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = m_stop;
	}
	//difference between now and start_
	now = diff(now, m_start);
	return now.tv_sec*1000000 + (now.tv_nsec + 500) / 1000;
}

void Timer::stop(){
	if( is_running() ){
		clock_gettime(CLOCK_REALTIME, &m_stop);
	}
}

#endif
