/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_CHRONO_CLOCK_TIME_HPP_
#define SAPI_CHRONO_CLOCK_TIME_HPP_

#include <mcu/types.h>
#include <time.h>

#include "../api/ChronoObject.hpp"
#include "Time.hpp"
#include "../var/String.hpp"

namespace chrono {

/*! \brief Clock Time Class
 * \details The ClockTime class is a helper class for using
 * struct timespec times. This is a 64-bit time with a 32-bit
 * seconds element and a 32-bit nanoseconds element.
 *
 *
 */
class ClockTime {
public:

	/*! \details Constructs a ClockTime object from seconds and nanoseconds.
	  *
	  *
	  */
	ClockTime(
			const Seconds & seconds,
			const Nanoseconds & nanoseconds
			){
		assign(seconds, nanoseconds);
	}

	/*! \details Constructs a clock time object based on the timespec. */
	ClockTime(
			const struct timespec & nano_time
			)
		: m_value(nano_time){}

	/*! \details Contructs an object from a micro time object. */
	explicit ClockTime(
			const MicroTime & micro_time
			);

	/*! \details Constructs a zero value ClockTime object. */
	ClockTime(){
		reset();
	}

	static ClockTime from_seconds(u32 seconds){
		return ClockTime(Seconds(seconds), Nanoseconds(0));
	}

	/*! \details Resets the value of the clock to zero. */
	ClockTime& reset(){
		m_value.tv_sec = 0; m_value.tv_nsec = 0;
		return *this;
	}

	/*! \details Sets the value of the clock time.
	  *
	  *
	  */
	ClockTime& set(
			const Seconds & seconds,
			const Nanoseconds & nanoseconds
			){
		m_value.tv_sec = seconds.seconds();
		m_value.tv_nsec = nanoseconds.nanoseconds();
		return *this;
	}

	ClockTime& set_seconds(
			u32 seconds
			){
		m_value.tv_sec = seconds;
		return *this;
	}

	ClockTime& set_nanoseconds(
			u32 value
			){
		m_value.tv_nsec = value;
		return *this;
	}

	ClockTime & operator << (const Seconds & seconds){
		m_value.tv_sec = seconds.seconds();
		return *this;
	}

	ClockTime & operator << (const Nanoseconds & nanoseconds){
		m_value.tv_nsec = nanoseconds.nanoseconds();
		return *this;
	}

	/*! \details Returns a pointer to the struct timespec.
	  *
	  * This allows the object to be passed directly to functions
	  * that required a pointer to struct timespec (read-only)
	  *
	  */
	operator const struct timespec * () const { return &m_value; }

	/*! \details Returns a pointer to the struct timespec.
	  *
	  * This allows the object to be passed directly to functions
	  * that required a pointer to struct timespec (read-write)
	  *
	  */
	operator struct timespec * (){ return &m_value; }

	/*! \details Returns true if the time is set to a valid value.
	  *
	  */
	bool is_valid() const {
		return *this != invalid();
	}

	/*! \details Returns a MicroTime object set to the invalid time. */
	static ClockTime invalid(){ return ClockTime(Seconds(-1), Nanoseconds(-1)); }

	/*! \details Returns true if this is greater than \a a. */
	bool operator > (const ClockTime & a) const;
	/*! \details Returns true if this is less than \a a. */
	bool operator < (const ClockTime & a) const;
	/*! \details Returns true if this is greater than or equal to \a a. */
	bool operator >= (const ClockTime & a) const;
	/*! \details Returns true if this is less than or equal to \a a. */
	bool operator <= (const ClockTime & a) const;
	/*! \details Returns true if this is equal to \a a. */
	bool operator == (const ClockTime & a) const;
	/*! \details Returns true if this is not equal to \a a. */
	bool operator != (const ClockTime & a) const;

	/*! \details Returns the sum of this object and \a a. */
	ClockTime operator + (const ClockTime & a) const { return add(*this, a); }
	/*! \details Returns the difference of this object and \a a. */
	ClockTime operator - (const ClockTime & a) const { return subtract(*this, a); }

	/*! \details Returns the age of this object as a ClockTime value.
	 *
	 * This will read the current time using Clock::get_time() and returns
	 * the difference between the present time and the time held by this object.
	 *
	 * ```
	 * #include <sapi/chrono.hpp>
	 *
	 * ClockTime instant = Clock::get_time();
	 * ClockTime::wait_seconds(5);
	 * printf("Age is %d\n", instant.age().seconds());
	 * ```
	 *
	 */
	ClockTime age() const;

	/*! \details Adds \a to this and assigned to this. */
	ClockTime & operator += (const ClockTime & a){
		*this = add(*this, a);
		return *this;
	}

	/*! \details Subracts \a from this and assigned to this. */
	ClockTime & operator -= (const ClockTime & a){
		//subtract nano time
		*this = subtract(*this, a);
		return *this;
	}

	/*! \details Returns the seconds component. */
	s32 seconds() const { return m_value.tv_sec; }
	/*! \details Returns the nanoseconds component. */
	s32 nanoseconds() const { return m_value.tv_nsec; }

	var::String get_unique_string() const {
		return var::String().format("%ld%09ld", seconds(), nanoseconds());
	}

	var::String get_string() const {
		return var::String().format("%ld.%09ld", seconds(), nanoseconds());
	}

	/*! \details Returns a pointer to a strut timespec. */
	struct timespec * timespec(){ return &m_value; }
	/*! \details Returns a pointer to a strut timespec (read-only). */
	const struct timespec * timespec() const { return &m_value; }


private:
	void assign(const Seconds & seconds, const Nanoseconds & nanoseconds);
	static ClockTime add(const ClockTime & a, const ClockTime & b);
	static ClockTime subtract(const ClockTime & a, const ClockTime & b);

	struct timespec m_value;
};

}

namespace sys {
class Printer;
}

sys::Printer & operator << (sys::Printer& printer, const chrono::ClockTime & a);

#endif /* SAPI_CHRONO_CLOCK_TIME_HPP_ */
