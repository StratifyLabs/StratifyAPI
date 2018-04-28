/*
 * Sched.hpp
 *
 *  Created on: Jul 27, 2016
 *      Author: tgil
 */

#ifndef SYS_SCHED_HPP_
#define SYS_SCHED_HPP_

#include <unistd.h>
#include <sched.h>
#include "../api/SysObject.hpp"

namespace sys {

/*! \brief Sched Class
 * \details The Sched class is used
 * to change the scheduing policy of the current
 * thread or process.
 *
 */
class Sched : public api::SysInfoObject {
public:

	enum policy {
		RR /*! Round Robin style (task yields periodically) */ = SCHED_RR,
		FIFO /*! First in, first out (task won't yield until it calls Sched::yield(), sleeps or uses Sync IO) */ = SCHED_FIFO,
		OTHER = SCHED_OTHER
	};

	/*! \details Yields the processor to another thread or process.
	 *
	 * It is the same as calling sched_yield().
	 *
	 */
	static void yield(){ sched_yield(); }

	/*! \details Gets the max priority for the specified policy. */
	static int get_priority_max(enum policy value);
	/*! \details Gets the min priority for the specified policy. */
	static int get_priority_min(enum policy value);
	/*! \details Gets the max priority for the specified policy. */
	static int get_priority(pid_t pid);

	/*! \details Gets the current proccess ID (equivalent to getpid()). */
	static pid_t get_pid(){ return getpid(); }

	/*! \details Gets the RR interval for the pid.
	 *
	 * @param pid  The process ID
	 * @return The RR interval in microseconds
	 */
	static int get_rr_interval(pid_t pid);

	/*! \details Sets the scheduler with the given parameters.
	 *
	 * @param pid The process ID
	 * @param value The polic (such as Sched::FIFO)
	 * @param priority The priority (higher is higher priority)
	 * @return Zero on success of -1 with errno set
	 */
	static int set_scheduler(pid_t pid, enum policy value, int priority);

};

} /* namespace sys */

#endif /* SYS_SCHED_HPP_ */
