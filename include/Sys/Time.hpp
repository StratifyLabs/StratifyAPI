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
	/*! \details Construct using current time */
	Time();
	/*! \details Construct using an amount of time */
	Time(u32 sec, u32 min = 0, u32 hour = 0);

	inline operator u32(){ return m_time; }


	/*! \details Add to the current value */
	Time& operator+=(const Time & a);
	/*! \details Subtract another time value */
	Time& operator=(const Time & a);
	/*! \details Assign another time value */
	Time& operator=(u32 a);
	/*! \details Subtract from the current value */
	Time& operator-=(const Time & a);

	static int set_time_of_day(const Time & t);
	int set_time_of_day();

	/*! \details This method sets the time to the current time.
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
	void set_value(u32 hour, u32 min, u32 sec);

	/*! \details Set the current value
	 *
	 * @param tm Number of seconds since epoch
	 */
	inline void set_value(time_t tm){ m_time = tm; }

	/*! \details Get the name of the month */
	const char * month_name() const;

	/*! \details The time value (number of seconds since epoch) */
	inline time_t value() const { return m_time; }

	/*! \details Seconds (from 0 to 59) */
	u32 second() const;
	/*! \details Minutes (from 0 to 59) */
	u32 minute() const;
	/*! \details Hours (from 0 to 23) */
	u32 hour() const;

	/*! \details Day of Month (from 1 to 31) */
	u32 get_day() const;
	/*! \details Day of Week (from 1 to 7) */
	u32 get_weekday() const;
	/*! \details Day of the year (1 to 366) */
	u32 get_yearday() const;
	/*! \details Month (from 1 to 12) */
	u32 get_month() const;
	/*! \details Year (e.g. 2014) */
	u32 get_year() const;

	/*! \details Convert the time to a struct tm */
	struct tm get_tm() const;


private:
	time_t m_time;

};

};



#endif /* TIME_HPP_ */
