/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_CHRONO_CLOCK_HPP_
#define SAPI_CHRONO_CLOCK_HPP_

#include "../api/ChronoObject.hpp"
#include "ClockTime.hpp"

namespace chrono {

/*! \brief Clock Class
 * \details The clock class is used for reading
 * the system CPU clocks.
 *
 * It reads out a chrono::ClockTime object which is
 * a 64-bit value with seconds and nanoseconds based
 * on struct timeval.
 *
 */
class Clock {
public:

	enum clock_id {
		clock_id_realtime /*! Realtime clock ID used with get_time() and get_resolution() */ = CLOCK_REALTIME
	};

	/*! \details Returns the present value of the specified clock.
	 *
	 * @return The time as a ClockTime object.
	 *
	 * ```
	 * #include <sapi/chrono.hpp>
	 *
	 * ClockTime now;
	 * now = Clock::get_time();
	 *
	 * ```
	 *
	 *
	 */
	static ClockTime get_time(enum clock_id clock_id = clock_id_realtime);

	/*! \details Gets the resolution of the specified clock. */
	static ClockTime get_resolution(enum clock_id clock_id = clock_id_realtime);
};

}

#endif // SAPI_CHRONO_CLOCK_HPP_
