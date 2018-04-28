//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <unistd.h>
#include <time.h>
#include "chrono/MicroTimer.hpp"
using namespace chrono;


void api::ChronoWorkObject::wait_seconds(u32 timeout){
    sleep(timeout);
}

void api::ChronoWorkObject::wait_microseconds(u32 duration){
    u32 seconds = duration / 1000000UL;
    if( seconds > 0 ){ sleep(seconds); }
    duration = duration - seconds * 1000000UL;
    usleep(duration);
}

void MicroTimer::wait(const MicroTime & micro_time){
    wait_microseconds(micro_time.microseconds());
}

#if !defined __link

MicroTimer::MicroTimer() { reset(); }

void MicroTimer::reset(){
    m_start.reset();
    m_stop.reset();
    //when stop.seconds() value is 0, the timer is in reset mode
    //when stop.seconds() value is -1, the timer is currently running
}

void MicroTimer::restart(){
    m_start.get_time();
    m_stop.set(-1, 0);
}


void MicroTimer::start(){
    if( is_running() == false ){
        restart();
    }
}

void MicroTimer::resume(){
    ClockTime new_start;
    ClockTime now;

    if( m_stop.seconds() < 0 ){
        return; //timer is not stopped
    }

    //if timer has been stopped, then resume counting
    if( m_start.seconds() + m_start.nanoseconds() ){ //start is non-zero
        new_start = m_stop - m_start;
        now.get_time();
        m_start = now - new_start;
        m_stop.set(-1, 0);
    } else {
        //if timer is not running then start it
        restart();
    }
}

u32 MicroTimer::calc_sec() const {
    ClockTime now;

    if( m_stop.seconds() < 0 ){
        now.get_time();
    } else {
        now = m_stop;
    }
    //difference between now and start time
    return now.seconds() - m_start.seconds();
}

MicroTime MicroTimer::calc_value() const {
    ClockTime now;
    if( m_stop.seconds() < 0 ){
        now.get_time();
    } else {
        now = m_stop;
    }
    //difference between now and start_
    now -= m_start;
    return MicroTime(now);
}

void MicroTimer::stop(){
    if( is_running() ){
        m_stop.get_time();
    }
}

#endif
