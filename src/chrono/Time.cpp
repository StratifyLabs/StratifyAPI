//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal.hpp"
#include "chrono/Time.hpp"
#if defined __StratifyOS__
#include <sos/dev/rtc.h>
#endif

using namespace chrono;

const char * month_names[] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

/*! \brief Construct using current time */
Time::Time(){
	m_time = 0;
}

/*! \brief Construct using an amount of time */
Time::Time(
		const Seconds & sec,
		const Minutes &  min,
		const Hours &  hour){
	set_time(sec, min, hour);
}

Time& Time::operator+=(const Time & a){
	m_time += a.time();
	return *this;
}

Time& Time::operator=(const Time & a){
	m_time = a.time();
	return *this;
}

Time& Time::operator=(u32 a){
	m_time = a;
	return *this;
}

Time& Time::operator-=(const Time & a){
	m_time -= a.time();
	return *this;
}

int Time::set_time_of_day(const Time & t){
#if defined __StratifyOS__
	int ret;
	int fd = ::open("/dev/rtc", O_RDWR);
	if( fd < 0 ){
		return -1;
	}
	struct tm t_data = t.get_tm();
	ret = ::ioctl(fd, I_RTC_SET, &t_data);
	close(fd);
	return ret;
#else
	return -1;
#endif
}

int Time::set_time_of_day(){
	return set_time_of_day(*this);
}


Time Time::get_time_of_day(){
	time_t t = ::time(0);
	if( t < 962668800 ){
		t = 962668800;
	}
	return Time(t);
}

void Time::set_time(
		const Seconds & sec,
		const Minutes & min,
		const Hours & hour
		){
	m_time = sec.seconds() +
			min.minutes()*60 +
			hour.hours()*3600;
}

u32 Time::second() const {
	return m_time % 60;
}

u32 Time::minute() const {
	return (m_time % 3600) / 60;
}

u32 Time::hour() const {
	return m_time / 3600 % 24;
}

u32 Time::day() const{
	return get_tm().tm_mday;
}
u32 Time::weekday() const{
	return get_tm().tm_wday;
}
u32 Time::yearday() const{
	return get_tm().tm_yday;
}
u32 Time::month() const{
	return get_tm().tm_mon + 1;
}
u32 Time::year() const{
	return get_tm().tm_year + 1900;
}

const var::ConstString Time::month_name() const {
	u32 mon = month();
	if( mon < 12 ){
		return month_names[mon];
	}
	return "";
}

struct tm Time::get_tm() const{
	struct tm time_struct;
#if defined __win32
	struct tm * ptr;
	ptr = gmtime(&m_time);
	time_struct = *ptr;
#else
	gmtime_r(&m_time, &time_struct);
#endif
	return time_struct;
}


