//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <unistd.h>
#include <time.h>
#include "Sys/Timer.hpp"
using namespace Sys;


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

Timer::Timer() {
	reset();
}

void Timer::reset(){
	m_start.tv_nsec = 0;
	m_start.tv_sec = 0;
	m_stop.tv_nsec = 0;
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

void Timer::start(){
	clock_gettime(CLOCK_REALTIME, &m_start);
	m_stop.tv_sec = -1;
}

void Timer::resume(){
	struct timespec new_start;
	struct timespec now;

	if( m_stop.tv_sec < 0 ){
		return; //timer is not stopped
	}

	new_start = diff(m_stop, m_start);
	clock_gettime(CLOCK_REALTIME, &now);
	m_start = diff(now, new_start);
	m_stop.tv_sec = -1;
}


bool Timer::is_running(){
	if( m_stop.tv_sec == -1 ){
		return true;
	}
	return false;
}

u32 Timer::calc_sec(){
	struct timespec now;
	if( m_stop.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = m_stop;
	}
	//difference between now and start_
	return now.tv_sec - m_start.tv_sec;
}

u32 Timer::calc_msec(){
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

u32 Timer::calc_usec(){
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
	clock_gettime(CLOCK_REALTIME, &m_stop);
}

#endif
