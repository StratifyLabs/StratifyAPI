/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CHRONO_TIME_HPP_
#define SAPI_CHRONO_TIME_HPP_


#include <ctime>
#include "../api/ChronoObject.hpp"
#include "../var/ConstString.hpp"
#include "ClockTime.hpp"

namespace chrono {

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
class Time : public api::ChronoInfoObject {
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
	  * @param t Time to use to set the system time
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
	  * @param hour Number of hours
	  * @param min Number of minutes
	  * @param sec Number of seconds
	  *
	  * This Time object will hold a duration of time rather than a
	  * calendar time.
	  *
	  */
	void set_time(
			const Seconds & sec,
			const Minutes & min,
			const Hours & hour
			);

	/*! \details Sets the current value.
	  *
	  * @param tm Number of seconds since epoch
	  */
	void set_time(time_t tm){ m_time = tm; }

	/*! \details Gets the name of the month. */
	const var::ConstString month_name() const;

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
