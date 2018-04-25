//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <unistd.h>
#include <time.h>
#include "chrono/MicroTimer.hpp"
using namespace chrono;


void MicroTimer::wait_seconds(u32 timeout){
    sleep(timeout);
}

void MicroTimer::wait(const MicroTime & micro_time){
    u32 timeout = micro_time.usec();
    u32 sec = micro_time.sec();
    if( sec > 0 ){ sleep(sec); }
    timeout = timeout - sec * 1000000UL;
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

MicroTimer::MicroTimer() { reset(); }

void MicroTimer::reset(){
    m_start.tv_nsec = 0;
    m_start.tv_sec = 0;
    m_stop.tv_nsec = 0;

    //when this value is 0, the timer is in reset mode
    //when this value is -1, the timer is currently running
    m_stop.tv_sec = 0;
}


int MicroTimer::get_clock_microseconds(){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec*1000000 + (now.tv_nsec) / 1000;
}

int MicroTimer::get_clock_milliseconds(){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec*1000 + (now.tv_nsec) / 1000000;
}

int MicroTimer::get_clock_seconds(){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec;
}

void MicroTimer::restart(){
    clock_gettime(CLOCK_REALTIME, &m_start);
    m_stop.tv_sec = -1;
}


void MicroTimer::start(){
    if( is_running() == false ){
        restart();
    }
}

void MicroTimer::resume(){
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

u32 MicroTimer::calc_sec() const {
    struct timespec now;

    if( m_stop.tv_sec < 0 ){
        clock_gettime(CLOCK_REALTIME, &now);
    } else {
        now = m_stop;
    }
    //difference between now and start time
    return now.tv_sec - m_start.tv_sec;
}

MicroTime MicroTimer::calc_value() const {
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

void MicroTimer::stop(){
    if( is_running() ){
        clock_gettime(CLOCK_REALTIME, &m_stop);
    }
}

#endif
