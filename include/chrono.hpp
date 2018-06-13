/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef CHRONO_HPP_
#define CHRONO_HPP_

/*! \brief Chrono (managing time, timers and clocks)
 *
 * The chrono namespace includes the following time-measurement
 * objects.
 *
 * - MicroTime: 32-bit value for microseconds
 * - ClockTime: 64-bit value for seconds and nanoseconds (clock as in CPU clock)
 * - Time: 32-bit value in seconds (basically a time_t object)
 *
 * The parent object for all chrono items is api::ChronoWorkObject or api::ChronoInfoObject.
 * Both of the top level objects provide a way to insert a static delay.
 *
 * \code
 *
 * #include <sapi/chrono.hpp>
 *
 * Timer::wait_seconds(1); //wait for one second -- Timer inherits ChronoWorkObject so it can use the wait methods statically
 * Timer::wait_milliseconds(100);
 * Timer::wait_microseconds(100);
 *
 * ClockTime clock_time;
 * Timer::wait(clock_time); //wait based on a clock time value
 *
 * MicroTime micro_time;
 * micro_time.set_milliseconds(100);
 * Timer::wait(micro_time); //wait based on a micro time value
 *
 * Time time(3, 2, 1); //3 seconds, 2 minutes, 1 hour
 * Timer::wait(time); //waits for 3 + 2*60 + 1*3600 seconds -- days, months, years are ignored
 *
 * \endcode
 *
 *
 */
namespace chrono {}

#include "chrono/ClockTime.hpp"
#include "chrono/MicroTime.hpp"
#include "chrono/Timer.hpp"
#include "chrono/Time.hpp"
#include "chrono/Clock.hpp"

#include "chrono/MicroTimer.hpp"

using namespace chrono;


#endif /* CHRONO_HPP_ */
