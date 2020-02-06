/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_API_CHRONO_OBJECT_HPP_
#define SAPI_API_CHRONO_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace chrono{
class Microseconds;
class ClockTime;
class Time;
}

namespace chrono {
/*! \details Delays based on a chrono::Microseconds value.
 *
 * ```
 * //md2code:main
 *
 * wait(Microseconds(100));
 *
 * //Milliseconds, Seconds, and Nanoseconds will convert to Microseconds automatically
 * wait(Seconds(3));
 * wait(Milliseconds(3000));
 * wait(Nanoseconds(60000));
 * wait(Nanoseconds(999)); //this won't wait because it converts to 0 microseconds
 * ```
 *
 * This method will yield control of the processor to another thread. So
 * the delay time may go over based on scheduling constraints.
 *
 */
void wait(const chrono::Microseconds & microseconds);
}

namespace api {

}


#endif // SAPI_API_CHRONO_OBJECT_HPP_
