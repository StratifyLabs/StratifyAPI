/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_CHRONO_TIME_HPP_
#define SAPI_CHRONO_TIME_HPP_


#include <ctime>
#include "../var/String.hpp"
#include "../api/ChronoObject.hpp"

namespace chrono {

class Microseconds;

class Hours {
public:
	explicit Hours(s32 value){ m_value = value; }
	u32 hours() const { return m_value; }
	static Hours invalid(){ return Hours((u32)-1); }
private:
	u32 m_value;
};

class Minutes {
public:
	explicit Minutes(s32 value){ m_value = value; }
	u32 minutes() const { return m_value; }
	static Minutes invalid(){ return Minutes((u32)-1); }
private:
	u32 m_value;
};


class Seconds {
public:
	explicit Seconds(s32 value){ m_value = value; }
	u32 seconds() const { return m_value; }
	static Seconds invalid(){ return Seconds((u32)-1); }
private:
	u32 m_value;
};

class Milliseconds {
public:
	explicit Milliseconds(s32 value){ m_value = value; }
	u32 milliseconds() const { return m_value; }
	static Milliseconds invalid(){ return Milliseconds((u32)-1); }
private:
	u32 m_value;
};

class Nanoseconds {
public:
	explicit Nanoseconds(s32 value){ m_value = value; }
	u32 nanoseconds() const { return m_value; }
	static Nanoseconds invalid(){ return Nanoseconds((u32)-1); }
private:
	u32 m_value;
};

class Timer;
class ClockTime;

/*! \details Defines the type for a chrono::MicroTime value. */
typedef u32 micro_time_t;

/*! \brief MicroTime Class
 * \details The MicroTime class is used for keeping track
 * of microsecond accurate time intervals. It uses
 * a 32-bit value so it is good for 4 billion microseconds
 * (or about 66 minutes).
 *
 * It is very handy for converting between microseconds,
 * milliseconds, and seconds. It also serves
 * to remove ambiguity when specifying short time intervals.
 *
 * For example:
 * ```
 * void set_period(const Microseconds & microseconds); //un-ambiguous and nice code completion
 * void set_period(u32 value); //the units here are not clear
 * void set_period_milliseconds(u32 value); //this is better but adds complexity
 * ```
 *
 *
 * ```
 * //md2code:include
 * #include <sapi/chrono.hpp>
 * ```
 *
 */
class Microseconds {
public:

	/*! \details Constructs a Microseconds object using a u32 microsecond value.
	 *
	 *
	 * The default initial value is zero.
	 *
	 * ```
	 * //md2code:main
	 * //must be explicitly constructed
	 * Microseconds duration = Microseconds(5);
	 * printf("Duration is %ld microseconds\n", duration.microseconds());
	 * ```
	 *
	 */
	explicit Microseconds(u32 microseconds = 0){ m_value_microseconds = microseconds; }

	/*! \details Constructs a Microseconds object from a chrono::ClockTime value. */
	Microseconds(const ClockTime & clock_time);

	/*! \details Constructs a Microseconds object from a chrono::Seconds value.
	 *
	 * ```
	 * //md2code:main
	 * Microseconds duration = Seconds(5); //converted to microseconds
	 * printf("Duration is %ld microseconds\n", duration.microseconds());
	 * ```
	 *
	 */
	Microseconds(const Seconds & seconds){
		m_value_microseconds = seconds.seconds()*1000000UL;
	}


	/*! \details Constructs a Microseconds object from a chrono::Milliseconds value.
	 *
	 * ```
	 * //md2code:main
	 * Microseconds duration = Milliseconds(3000); //converted to microseconds
	 * printf("Duration is %ld microseconds\n", duration.microseconds());
	 * ```
	 *
	 */
	Microseconds(const Milliseconds & milliseconds){
		m_value_microseconds = milliseconds.milliseconds()*1000UL;
	}

	/*! \details Constructs a Microseconds object from a chrono::Nanoseconds value.
	 *
	 * ```
	 * //md2code:main
	 * Microseconds duration = Nanoseconds(3000); //converted to microseconds
	 * Microseconds zero = Nanoseconds(999); //converted using truncation -- goes to zero
	 * printf("Duration is %ld microseconds\n", duration.microseconds());
	 * printf("Zero is %ld microseconds\n", zero.microseconds());
	 * ```
	 *
	 */
	Microseconds(const Nanoseconds & nanoseconds){
		m_value_microseconds = nanoseconds.nanoseconds() / 1000;
	}

	/*! \details Constructs a Microseconds object from the current value of a chrono::Timer. */
	Microseconds(const Timer & timer);


	/*! \details Returns true if the time is set to a valid value.
	  *
	  */
	bool is_valid() const {
		return m_value_microseconds != (u32)-1;
	}

	/*! \details Returns a MicroTime object set to the invalid time value. */
	static Microseconds invalid(){ return Microseconds((u32)-1); }


	/*! \details Assignment addition to another MicroTime object. */
	Microseconds & operator += (const Microseconds & a){
		m_value_microseconds += a.microseconds();
		return *this;
	}

	/*! \details Assignment addition to another MicroTime object. */
	Microseconds & operator -= (const Microseconds & a){
		m_value_microseconds -= a.microseconds();
		return *this;
	}

	Microseconds operator + (const Microseconds & a) const {
		return Microseconds(microseconds() + a.microseconds());
	}

	Microseconds operator - (const Microseconds & a) const {
		return Microseconds(microseconds() - a.microseconds());
	}

	/*! \details Compares equality to another MicroTime object. */
	bool operator == (const Microseconds & a ) const { return microseconds() == a.microseconds(); }

	/*! \details Compares inequality to another MicroTime object. */
	bool operator != (const Microseconds & a ) const { return microseconds() != a.microseconds(); }

	/*! \details Compares > to another MicroTime object. */
	bool operator > (const Microseconds & a ) const { return microseconds() > a.microseconds(); }

	/*! \details Compares < to another MicroTime object. */
	bool operator < (const Microseconds & a ) const { return microseconds() < a.microseconds(); }

	/*! \details Compares >= to another MicroTime object. */
	bool operator >= (const Microseconds & a ) const { return microseconds() >= a.microseconds(); }

	/*! \details Compares <= to another MicroTime object. */
	bool operator <= (const Microseconds & a ){ return microseconds() <= a.microseconds(); }

	Microseconds & operator << (const Seconds & a){ return (*this) = a; }
	Microseconds & operator << (const Milliseconds & a){ return (*this) = a; }
	Microseconds & operator << (const Microseconds & a){ return (*this) = a; }
	Microseconds & operator << (const Nanoseconds & a){ return (*this) = a; }


	/*! \details Returns the value in seconds. */
	u32 seconds() const { return microseconds() / 1000000UL; }

	/*! \details Returns the value in microseconds. */
	micro_time_t microseconds() const { return m_value_microseconds; }

	/*! \details Returns the value in milliseconds. */
	u32 milliseconds() const { return microseconds() / 1000UL; }

	/*! \details Waits for the value of the microtime.
	  *
	  */
	void wait() const;

private:
	micro_time_t m_value_microseconds;
};

using MicroTime = Microseconds;

/*! \brief Time Class
 * \details This class is for accessing the current time as well
 * as adding and subtracting times and dates.
 *
 * The time is based on the value of the RTC in the system. The RTC
 * will keep the same value even when the device is reset. This class
 * should provide accurate access to the current data and time.
 *
 * It holds a c style time_t value that can be managed which is
 * a 32-bit value with accuracy to the second.
 *
 * \code
 * Time now; //current time
 * Time ten_minutes(0, 10, 0); //duration of 10 minutes
 *
 * now += ten_minutes; //adds ten minutes to now
 * \endcode
 *
 */
class Time {
public:
	/*! \details Constructs using current time. */
	Time();

	explicit Time(const time_t & t){ m_time = t; }

	/*! \details Constructs using an amount of time. */
	Time(
			const Seconds & seconds,
			const Minutes & minutes = Minutes(0),
			const Hours & hours = Hours(0)
			);

	Time(const Minutes & minutes){ m_time = minutes.minutes()*60; }
	Time(const Hours & hours){ m_time = hours.hours()*3600; }

	Time(const var::String & time_string, const var::String& format = "%Y-%m-%d %H:%M:%S");

	bool is_valid(){ return m_time != 0; }

	static Time current_time(){ return get_time_of_day(); }
	static Time from_seconds(u32 value){ return Time(value); }
	static Time from_minutes(u32 value){ return Time(Seconds(0), Minutes(value), Hours(0)); }
	static Time from_hours(u32 value){ return Time(Seconds(0), Minutes(0), Hours(value)); }
	static Time from_days(u32 value){ return Time(Seconds(0), Minutes(0), Hours(value*24)); }
	static Time from_weeks(u32 value){ return Time(Seconds(0), Minutes(0), Hours(value*24*7)); }

	Time operator + (const Time & a ) const {
		Time result;
		result.m_time = m_time + a.m_time;
		return result;
	}

	Time operator - (const Time & a ) const {
		Time result;
		result.m_time = m_time - a.m_time;
		return result;
	}

	bool operator == (const Time & a ) const { return m_time == a.m_time; }
	bool operator != (const Time & a ) const { return m_time != a.m_time; }
	bool operator > (const Time & a ) const { return m_time > a.m_time; }
	bool operator < (const Time & a ) const { return m_time < a.m_time; }
	bool operator >= (const Time & a ) const { return m_time >= a.m_time; }
	bool operator <= (const Time & a ) const { return m_time <= a.m_time; }

	Time age() const {
		return Time::get_time_of_day() - *this;
	}

	/*! \details Adds to the current value. */
	Time& operator+=(const Time & a);
	/*! \details Assigns another Time. */
	Time& operator=(const Time & a);
	/*! \details Assigns another time value (time_t). */
	Time& operator=(u32 a);
	/*! \details Subtracts from the current value. */
	Time& operator-=(const Time & a);

	/*! \details Sets the system time
	  *
	  */
	static int set_time_of_day(const Time & t);

	/*! \details Sets the system time to the time stored in this object. */
	int set_time_of_day();

	/*! \details Assigns the system time of day to the time stored in this object
	  * and returns the current system time.
	  */
	static Time get_time_of_day();

	/*! \details Sets the value in Time to a number of seconds.
	  *
	  * This Time object will hold a duration of time rather than a
	  * calendar time.
	  *
	  */
	Time& set_time(
			const Seconds & sec,
			const Minutes & min,
			const Hours & hour
			);


	/*! \details Sets the current value.
	  *
	  */
	Time& set_time(time_t tm){
		m_time = tm;
		return *this;
	}

	/*! \details Gets the name of the month. */
	const char * month_name() const;

	/*! \details Returns the time value (number of seconds since epoch). */
	time_t ctime() const { return m_time; }
	time_t time() const { return m_time; }

	/*! \details Returns seconds (from 0 to 59). */
	u32 second() const;
	/*! \details Returns minutes (from 0 to 59). */
	u32 minute() const;
	/*! \details Returns hours (from 0 to 23). */
	u32 hour() const;

	/*! \details Returns the day of month (from 1 to 31). */
	u32 day() const;
	/*! \details Returns the day of week (from 1 to 7). */
	u32 weekday() const;
	/*! \details Returns the day of the year (1 to 366). */
	u32 yearday() const;
	/*! \details Returns the month (from 1 to 12). */
	u32 month() const;
	/*! \details Returns the year (e.g. 2014) */
	u32 year() const;

	/*! \details Converts the time to a struct tm. */
	struct tm get_tm() const;


private:
	time_t m_time;

};

}



#endif /* SAPI_CHRONO_TIME_HPP_ */
