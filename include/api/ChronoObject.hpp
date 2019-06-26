#ifndef SAPI_API_CHRONO_OBJECT_HPP_
#define SAPI_API_CHRONO_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace chrono{
class MicroTime;
class ClockTime;
class Time;
}

namespace chrono {
/*! \details Wait for the specified number of seconds. */
void wait_seconds(u32 timeout);

/*! \details This method will delay the specified number of milliseconds.
  *
  * @param timeout Time to wait in milliseconds
  */
void wait_milliseconds(u32 timeout);
/*! \details This method will delay the specified number of microseconds.
  *
  * @param timeout Time to wait in microseconds
  */
void wait_microseconds(u32 duration);
/*! \details This method delays based on a chrono::ClockTime value. */
void wait(const chrono::ClockTime & clock_time);
/*! \details This method delays based on a chrono::MicroTime value. */
void wait(const chrono::MicroTime & micro_time);
/*! \details This method delays based on a chrono::Time value. */
void wait(const chrono::Time & time);
}

namespace api {

/*! \brief Chrono Information Object
 * \details
 *
 * \sa chrono namespace
 */
class ChronoInfoObject : public virtual InfoObject {
public:
	static void wait_seconds(u32 timeout){ chrono::wait_seconds(timeout); }
	static void wait_milliseconds(u32 timeout){ chrono::wait_milliseconds(timeout); }
	static void wait_microseconds(u32 timeout){ chrono::wait_microseconds(timeout); }

	static void wait(const chrono::ClockTime & clock_time){ chrono::wait(clock_time); }
	static void wait(const chrono::MicroTime & micro_time){ chrono::wait(micro_time); }
	static void wait(const chrono::Time & time){ chrono::wait(time); }
};

/*! \brief Chrono Work Object
 *
 * \details
 *
 * \sa chrono namespace
 */
class ChronoWorkObject : public virtual WorkObject {
public:

	/*! \details See chrono::ChronoInfoObject::wait_seconds();
	  */
	static void wait_seconds(u32 timeout){ chrono::wait_seconds(timeout); }

	/*! \details See chrono::ChronoInfoObject::wait_milliseconds();
	  */
	static void wait_milliseconds(u32 timeout){ chrono::wait_milliseconds(timeout); }

	/*! \details See chrono::ChronoInfoObject::wait_microseconds();
	  */
	static void wait_microseconds(u32 timeout){ chrono::wait_microseconds(timeout); }

	/*! \details This method delays based on a chrono::ClockTime value. */
	static void wait(const chrono::ClockTime & clock_time){ chrono::wait(clock_time); }
	/*! \details This method delays based on a chrono::MicroTime value. */
	static void wait(const chrono::MicroTime & micro_time){ chrono::wait(micro_time); }
	/*! \details This method delays based on a chrono::Time value. */
	static void wait(const chrono::Time & time){ chrono::wait(time); }

};


}


#endif // SAPI_API_CHRONO_OBJECT_HPP_
