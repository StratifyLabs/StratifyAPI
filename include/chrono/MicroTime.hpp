/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef CHRONO_MICRO_TIME_HPP_
#define CHRONO_MICRO_TIME_HPP_

#include <mcu/types.h>
#include <time.h>

#include "../api/ChronoObject.hpp"
#include "ClockTime.hpp"

namespace chrono {

class Timer;

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
 * \code
 * void set_period(const MicroTime & micro_time); //un-ambiguous and nice code completion
 * void set_period(u32 value); //the units here are not clear
 * void set_period_milliseconds(u32 value); //this is better but adds complexity
 * \endcode
 *
 */
class MicroTime : public api::ChronoInfoObject {
public:

	/*! \details Constructs a MicroTime object using a u32 microsecond value.
	  *
	  * The default initial value is zero.
	  *
	  */
	explicit MicroTime(u32 microseconds = 0){ m_value_microseconds = microseconds; }
	//MicroTime(){ m_value_microseconds = 0; }

	/*! \details Constructs a MicroTime object from a chrono::ClockTime value. */
	MicroTime(const ClockTime & clock_time){
		m_value_microseconds = clock_time.seconds() * 1000000UL + (clock_time.nanoseconds() + 500) / 1000;
	}

	/*! \details Constructs a MicroTime object from the current value of a chrono::Timer. */
	MicroTime(const Timer & timer);

	/*! \details Create a MicroTime object from a second value. */
	static MicroTime from_seconds(u32 sec){ return MicroTime(sec*1000000UL); }


	/*! \details Create a MicroTime object from a millisecond value. */
	static MicroTime from_milliseconds(u32 msec){ return MicroTime(msec*1000UL); }


	/*! \details Create a MicroTime object from a microsecond value.
	  *
	  * The following objects
	  * will be initialized with the same value because the constructor
	  * value assumes microseconds.
	  *
	  * \code
	  * MicroTime time_from_microseconds(MicroTime::from_microseconds(1000));
	  * MicroTime time_microseconds(1000);
	  * \endcode
	  *
	  */
	static MicroTime from_microseconds(u32 microseconds){ return MicroTime(microseconds); }

	/*! \cond */
	static MicroTime from_msec(u32 msec){ return from_milliseconds(msec); }
	static MicroTime from_sec(u32 sec){ return from_seconds(1000000UL*sec); }
	static MicroTime from_usec(u32 usec){ return from_microseconds(usec); }
	/*! \endcond */

	/*! \details Returns true if the time is set to a valid value.
	  *
	  */
	bool is_valid() const {
		return m_value_microseconds != (u32)-1;
	}

	/*! \details Returns a MicroTime object set to the invalid time value. */
	static MicroTime invalid(){ return MicroTime((u32)-1); }


	/*! \details Assignment addition to another MicroTime object. */
	MicroTime & operator += (const MicroTime & micro_time){
		m_value_microseconds += micro_time.usec();
		return *this;
	}

	/*! \details Assignment addition to another MicroTime object. */
	MicroTime & operator -= (const MicroTime & micro_time){
		m_value_microseconds -= micro_time.usec();
		return *this;
	}

	/*! \details Compares equality to another MicroTime object. */
	bool operator == (const MicroTime & a ) const { return microseconds() == a.microseconds(); }

	/*! \details Compares inequality to another MicroTime object. */
	bool operator != (const MicroTime & a ) const { return microseconds() != a.microseconds(); }

	/*! \details Compares > to another MicroTime object. */
	bool operator > (const MicroTime & a ) const { return microseconds() > a.microseconds(); }

	/*! \details Compares < to another MicroTime object. */
	bool operator < (const MicroTime & a ) const { return microseconds() < a.microseconds(); }

	/*! \details Compares >= to another MicroTime object. */
	bool operator >= (const MicroTime & a ) const { return microseconds() >= a.microseconds(); }

	/*! \details Compares <= to another MicroTime object. */
	bool operator <= (const MicroTime & a ){ return microseconds() <= a.microseconds(); }

	/*! \details Sets the value of the time in seconds.
	  *
	  * @param sec The number of seconds.
	  *
	  */
	void set_seconds(u32 sec){ set_usec(sec*1000000UL); }

	/*! \details Sets the value of the time in milliseconds.
	  *
	  * @param msec The value to assign in milliseconds.
	  *
	  */
	void set_milliseconds(u32 msec){ set_usec(msec*1000UL); }

	/*! \details Sets the value of the time in microseconds.
	  *
	  * @param microseconds The value in microseconds
	  *
	  */
	void set_microseconds(micro_time_t microseconds){ m_value_microseconds = microseconds; }

	/*! \details Returns the value in seconds. */
	u32 seconds() const { return microseconds() / 1000000UL; }

	/*! \details Returns the value in microseconds. */
	micro_time_t microseconds() const { return m_value_microseconds; }

	/*! \details Returns the value in milliseconds. */
	u32 milliseconds() const { return usec() / 1000UL; }

	/*! \cond */
	void set_sec(u32 sec){ set_seconds(sec); }
	void set_msec(u32 msec){ set_milliseconds(msec); }
	void set_usec(micro_time_t usec){ set_microseconds(usec); }
	u32 sec() const { return seconds(); }
	u32 msec() const { return milliseconds(); }
	micro_time_t usec() const { return microseconds(); }
	/*! \endcond */

	/*! \details Waits for the value of the microtime.
	  *
	  */
	void wait() const { wait_microseconds( m_value_microseconds );}

private:
	micro_time_t m_value_microseconds;
};

}

#endif /* CHRONO_MICRO_TIME_HPP_ */
