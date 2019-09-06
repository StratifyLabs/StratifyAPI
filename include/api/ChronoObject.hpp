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
/*! \details This method delays based on a chrono::MicroTime value. */
void wait(const chrono::Microseconds & micro_time);
}

namespace api {

}


#endif // SAPI_API_CHRONO_OBJECT_HPP_
