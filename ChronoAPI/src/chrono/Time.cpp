/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include "hal.hpp"
#include "sys/Printer.hpp"
#include "chrono/Time.hpp"
#include "chrono/Timer.hpp"
#if defined __StratifyOS__
#include <sos/dev/rtc.h>
#endif

#if defined __win32
#include <time.h>
#include <iomanip>
#include <sstream>

static char* strptime(
		const char* s,
		const char* f,
		struct tm* tm) {
	// Isn't the C++ standard lib nice? std::get_time is defined such that its
	// format parameters are the exact same as strptime. Of course, we have to
	// create a string stream first, and imbue it with the current C locale, and
	// we also have to make sure we return the right things if it fails, or
	// if it succeeds, but this is still far simpler an implementation than any
	// of the versions in any of the C standard libraries.
	std::istringstream input(s);
	input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
	input >> std::get_time(tm, f);
	if (input.fail()) {
		return nullptr;
	}
	return (char*)(s + input.tellg());
}

#endif

sys::Printer& sys::operator << (sys::Printer& printer, const chrono::MicroTime & a){
	printer.key("duration", var::String::number(a.microseconds(), F32U "us"));
	return printer;
}

sys::Printer& sys::operator << (sys::Printer& printer, const chrono::Time & a){
	printer.key("time", F32U, a.time());
	return printer;
}

using namespace chrono;




Microseconds::Microseconds(const Timer & timer){
	m_value_microseconds = timer.microseconds();
}

Microseconds::Microseconds(const ClockTime & clock_time){
	m_value_microseconds = clock_time.seconds() * 1000000UL + (clock_time.nanoseconds() + 500) / 1000;
}

void Microseconds::wait() const {
	chrono::wait(*this);
}

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

Time::Time(
		const var::String & time_string,
		const var::String & format
		){
	struct tm tm = {0};
	if( strptime(time_string.cstring(), format.cstring(), &tm) != nullptr ){
		m_time = mktime(&tm);
	} else {
		m_time = 0;
	}
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

Time& Time::set_time(
		const Seconds & sec,
		const Minutes & min,
		const Hours & hour
		){
	m_time = sec.seconds() +
			min.minutes()*60 +
			hour.hours()*3600;
	return *this;
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

const char * Time::month_name() const {
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




