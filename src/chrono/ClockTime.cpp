#include "chrono/ClockTime.hpp"
#include "chrono/Clock.hpp"
#include "chrono/MicroTime.hpp"

using namespace chrono;


ClockTime ClockTime::age() const {
	return Clock::get_time() - *this;
}

ClockTime::ClockTime(const MicroTime & micro_time){
	m_value.tv_sec = micro_time.seconds();
	u32 microseconds = micro_time.microseconds() - m_value.tv_sec*1000000;
	m_value.tv_nsec = microseconds;
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

void ClockTime::assign(const Seconds & seconds, const Nanoseconds & nanoseconds){
	m_value.tv_sec = seconds.seconds();
	m_value.tv_nsec = nanoseconds.nanoseconds();
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
