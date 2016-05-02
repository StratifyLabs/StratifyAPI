/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef TIME_HPP_
#define TIME_HPP_

#include <ctime>
#include <mcu/types.h>

namespace Sys {

/*! \brief Time Class
 * \details This class is for accessing the current time.
 *
 */
class Time {
public:
	/*! \brief Construct using current time */
	Time();
	/*! \brief Construct using an amount of time */
	Time(u32 sec, u32 min = 0, u32 hour = 0);

	inline operator u32(){ return t; }

	Time& operator+=(const Time & a);
	Time& operator=(const Time & a);
	Time& operator=(u32 a);
	Time& operator-=(const Time & a);

	static int set_time_of_day(const Time & t);
	int set_time_of_day();

	/*! \brief Set the time to the current time.
	 * \details This method sets the time.
	 * @param value time_t to set (-1 for current time)
	 */
	void set_current();

	/*! \brief Set the time
	 * \details This method is for setting a finite time rather than
	 * an absolute time.
	 *
	 * @param hour Number of hours
	 * @param min Number of minutes
	 * @param sec Number of seconds
	 */
	void set(u32 hour, u32 min, u32 sec);

	const char * month_name() const;

	/*! \brief The time value (number of seconds since epoch) */
	inline time_t value() const { return t; }
	inline void set_value(time_t tm){ t = tm; }

	/*! \brief Seconds (from 0 to 59) */
	u32 second() const;
	/*! \brief Minutes (from 0 to 59) */
	u32 minute() const;
	/*! \brief Hours (from 0 to 23) */
	u32 hour() const;

	/*! \brief Day of Month (from 1 to 31) */
	u32 day() const;
	/*! \brief Day of Week (from 1 to 7) */
	u32 weekday() const;
	/*! \brief Day of the year (1 to 366) */
	u32 yearday() const;
	/*! \brief Month (from 1 to 12) */
	u32 month() const;
	/*! \brief Year (e.g. 2014) */
	u32 year() const;

	struct tm tm() const;

private:
	time_t t;

};

};



#endif /* TIME_HPP_ */
