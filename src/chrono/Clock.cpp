#include "chrono/Clock.hpp"

using namespace chrono;

ClockTime Clock::get_time(int clock_id){
    ClockTime clock_time;
#if defined __macosx

#else
    if( clock_gettime(clock_id, clock_time) < 0 ){
        clock_time = ClockTime::invalid();
    }
#endif
    return clock_time;
}


ClockTime Clock::get_resolution(int clock_id){
    ClockTime resolution;
#if defined __macosx


#else
    if( clock_getres(clock_id, resolution) < 0 ){
        resolution = ClockTime::invalid();
    }
#endif
    return resolution;
}
