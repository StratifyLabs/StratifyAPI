/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

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
