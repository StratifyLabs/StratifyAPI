#include "chrono/ClockTime.hpp"
#include "chrono/MicroTime.hpp"

using namespace chrono;


ClockTime::ClockTime(const MicroTime & micro_time){
    m_value.tv_sec = micro_time.seconds();
    u32 microseconds = micro_time.microseconds() - m_value.tv_sec*1000000;
    m_value.tv_nsec = microseconds;
}

int ClockTime::get_time(int clock_id){
    return set_error_number_if_error(clock_gettime(clock_id, &m_value));
}


ClockTime ClockTime::get_resolution(int clock_id){
    ClockTime resolution;
    clock_getres(CLOCK_REALTIME, &resolution.m_value);
    return resolution;
}



bool ClockTime::operator > (const ClockTime & a) const {
    if( m_value.tv_sec > a.m_value.tv_sec ){
        return true;
    } else if( m_value.tv_sec == a.m_value.tv_sec ){
        return m_value.tv_nsec > a.m_value.tv_nsec;
    }
    return false;
}

bool ClockTime::operator < (const ClockTime & a) const {
    if( m_value.tv_sec < a.m_value.tv_sec ){
        return true;
    } else if( m_value.tv_sec == a.m_value.tv_sec ){
        return m_value.tv_nsec < a.m_value.tv_nsec;
    }
    return false;
}

bool ClockTime::operator >= (const ClockTime & a) const {
    if( m_value.tv_sec >= a.m_value.tv_sec ){
        return true;
    } else if( m_value.tv_sec == a.m_value.tv_sec ){
        return m_value.tv_nsec >= a.m_value.tv_nsec;
    }
    return false;
}

bool ClockTime::operator <= (const ClockTime & a) const {
    if( m_value.tv_sec <= a.m_value.tv_sec ){
        return true;
    } else if( m_value.tv_sec == a.m_value.tv_sec ){
        return m_value.tv_nsec <= a.m_value.tv_nsec;
    }
    return false;
}

bool ClockTime::operator == (const ClockTime & a) const {
    if( (m_value.tv_sec == a.m_value.tv_sec) && (m_value.tv_nsec == a.m_value.tv_nsec) ){
        return true;
    }
    return false;
}

bool ClockTime::operator != (const ClockTime & a) const {
    if( (m_value.tv_sec != a.m_value.tv_sec) || (m_value.tv_nsec != a.m_value.tv_nsec) ){
        return true;
    }
    return false;
}

void ClockTime::assign(s32 seconds, s32 nanoseconds){
    m_value.tv_sec = seconds;
    m_value.tv_nsec = nanoseconds;
    if( m_value.tv_nsec > 1000000000 ){
        m_value.tv_sec++;
        m_value.tv_nsec -= 1000000000;
    }

    if( m_value.tv_nsec < 0 ){
        m_value.tv_sec--;
        m_value.tv_nsec += 1000000000;
    }
}

ClockTime ClockTime::add(const ClockTime & a, const ClockTime & b){
    ClockTime c;
    c.m_value.tv_sec = a.m_value.tv_sec + b.m_value.tv_sec;
    c.m_value.tv_nsec = a.m_value.tv_nsec + b.m_value.tv_nsec;
    if( c.m_value.tv_nsec > 1000000000 ){
        c.m_value.tv_nsec -= 1000000000;
        c.m_value.tv_sec++;
    }
    return c;
}

ClockTime ClockTime::subtract(const ClockTime & a, const ClockTime & b){
    ClockTime c;
    c.m_value.tv_sec = a.m_value.tv_sec - b.m_value.tv_sec;
    c.m_value.tv_nsec = a.m_value.tv_nsec - b.m_value.tv_nsec;
    if( c.m_value.tv_nsec < 0 ){
        c.m_value.tv_nsec += 1000000000;
        c.m_value.tv_sec--;
    }
    return c;
}
