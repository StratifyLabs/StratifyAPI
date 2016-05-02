//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <sys/time.h>
#include "Sys/Time.hpp"

using namespace Sys;


const char * month_names[] = {
		"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

/*! \brief Construct using current time */
Time::Time(){
	set_current();
}
/*! \brief Construct using an amount of time */
Time::Time(u32 sec, u32 min, u32 hour){
	set(sec, min, hour);
}

Time& Time::operator+=(const Time & a){
	t += a.value();
	return *this;
}

Time& Time::operator=(const Time & a){
	t = a.value();
	return *this;
}

Time& Time::operator=(u32 a){
	t = a;
	return *this;
}

Time& Time::operator-=(const Time & a){
	t -= a.value();
	return *this;
}

int Time::set_time_of_day(const Time & t){
	struct timeval tv;
	struct timezone tz;
	tv.tv_sec = t.value();
	tv.tv_usec = 0;
	return settimeofday(&tv, &tz);
}

int Time::set_time_of_day(){
	return set_time_of_day(*this);
}

void Time::set_current(){
	t = time(0);
}

void Time::set(u32 hour, u32 min, u32 sec){
	t = sec + min*60 + hour*3600;
}

u32 Time::second() const {
	return t % 60;
}

u32 Time::minute() const {
	return (t % 3600) / 60;
}

u32 Time::hour() const {
	return t / 3600 % 24;
}

u32 Time::day() const{
	return tm().tm_mday;
}
u32 Time::weekday() const{
	return tm().tm_wday;
}
u32 Time::yearday() const{
	return tm().tm_yday;
}
u32 Time::month() const{
	return tm().tm_mon;
}
u32 Time::year() const{
	return tm().tm_year + 1900;
}

const char * Time::month_name() const {
	u32 mon = month();
	if( mon < 12 ){
		return month_names[mon];
	}
	return "";
}

struct tm Time::tm() const{
	struct tm time_struct;
	gmtime_r(&t, &time_struct);
	return time_struct;
}

