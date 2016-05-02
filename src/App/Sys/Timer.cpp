//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <unistd.h>
#include <time.h>
#include "Sys/Timer.hpp"
using namespace Sys;

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

void Timer::reset(void){
	start_.tv_nsec = 0;
	start_.tv_sec = 0;
	stop_.tv_nsec = 0;
	stop_.tv_sec = 0;
}


int Timer::clock_usec(void){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec*1000000 + (now.tv_nsec) / 1000;
}

int Timer::clock_msec(void){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec*1000 + (now.tv_nsec) / 1000000;
}

int Timer::clock_sec(void){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec;
}



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

void Timer::start(void){
	clock_gettime(CLOCK_REALTIME, &start_);
	stop_.tv_sec = -1;
}

void Timer::resume(void){
	struct timespec new_start;
	struct timespec now;

	if( stop_.tv_sec < 0 ){
		return; //timer is not stopped
	}

	new_start = diff(stop_, start_);
	clock_gettime(CLOCK_REALTIME, &now);
	start_ = diff(now, new_start);
	stop_.tv_sec = -1;
}


bool Timer::is_running(void){
	if( stop_.tv_sec == -1 ){
		return true;
	}
	return false;
}

u32 Timer::sec(void){
	struct timespec now;
	if( stop_.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = stop_;
	}
	//difference between now and start_
	return now.tv_sec - start_.tv_sec;
}

u32 Timer::msec(void){
	struct timespec now;
	if( stop_.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = stop_;
	}
	//difference between now and start_
	now = diff(now, start_);
	return now.tv_sec*1000 + (now.tv_nsec + 500000) / 1000000;
}

u32 Timer::usec(void){
	struct timespec now;
	if( stop_.tv_sec < 0 ){
		clock_gettime(CLOCK_REALTIME, &now);
	} else {
		now = stop_;
	}
	//difference between now and start_
	now = diff(now, start_);
	return now.tv_sec*1000000 + (now.tv_nsec + 500) / 1000;
}

void Timer::stop(void){
	clock_gettime(CLOCK_REALTIME, &stop_);
}
