//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#if !defined __link

#include <sys/time.h>
#include <sos/dev/rtc.h>
#include "hal.hpp"
#include "sys/Time.hpp"

using namespace sys;


const char * month_names[] = {
		"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

/*! \brief Construct using current time */
Time::Time(){
	get_time_of_day();
}
/*! \brief Construct using an amount of time */
Time::Time(u32 sec, u32 min, u32 hour){
	set_value(sec, min, hour);
}

Time& Time::operator+=(const Time & a){
	m_time += a.value();
	return *this;
}

Time& Time::operator=(const Time & a){
	m_time = a.value();
	return *this;
}

Time& Time::operator=(u32 a){
	m_time = a;
	return *this;
}

Time& Time::operator-=(const Time & a){
	m_time -= a.value();
	return *this;
}

int Time::set_time_of_day(const Time & t){
	int ret;
	int fd = ::open("/dev/rtc", O_RDWR);
	if( fd < 0 ){
		return -1;
	}
	struct tm t_data = t.get_tm();
	ret = ::ioctl(fd, I_RTC_SET, &t_data);
	close(fd);
	return ret;

}

int Time::set_time_of_day(){
    return set_time_of_day(*this);
}

time_t Time::get_time_of_day(){
	m_time = time(0);
	if( m_time < 962668800 ){
		m_time = 962668800;
	}
	return m_time;
}

void Time::set_value(u32 hour, u32 min, u32 sec){
	m_time = sec + min*60 + hour*3600;
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

u32 Time::get_day() const{
	return get_tm().tm_mday;
}
u32 Time::get_weekday() const{
	return get_tm().tm_wday;
}
u32 Time::get_yearday() const{
	return get_tm().tm_yday;
}
u32 Time::get_month() const{
	return get_tm().tm_mon;
}
u32 Time::get_year() const{
	return get_tm().tm_year + 1900;
}

const char * Time::get_month_name() const {
	u32 mon = get_month();
	if( mon < 12 ){
		return month_names[mon];
	}
	return "";
}

struct tm Time::get_tm() const{
	struct tm time_struct;
	gmtime_r(&m_time, &time_struct);
	return time_struct;
}

#endif

