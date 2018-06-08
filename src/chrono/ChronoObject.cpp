#include <unistd.h>
#include "chrono/ClockTime.hpp"
#include "chrono/MicroTime.hpp"
#include "chrono/Time.hpp"
#include "api/ChronoObject.hpp"

using namespace api;

void ChronoInfoObject::wait_seconds(u32 timeout){ sleep(timeout); }

void ChronoInfoObject::wait_microseconds(u32 duration){
    u32 seconds = duration / 1000000UL;
    if( seconds > 0 ){ wait_seconds(seconds); }
    duration = duration - seconds * 1000000UL;
    usleep(duration);
}


void ChronoInfoObject::wait(const chrono::ClockTime & clock_time){
    wait_seconds(clock_time.seconds());
    wait_microseconds(clock_time.nanoseconds()/1000UL);
}

void ChronoInfoObject::wait(const chrono::MicroTime & micro_time){
    wait_microseconds(micro_time.microseconds());
}

void ChronoInfoObject::wait(const chrono::Time & time){
    wait_seconds(time.hour() * 3600UL + time.minute()*60UL + time.second());
}

