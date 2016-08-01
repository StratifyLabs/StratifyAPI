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

namespace sys {

/*! \brief Sched Class
 * \details This Class gives control over the
 * priority of a process.
 */
class Sched {
public:


	enum policy {
		RR /*! Round Robin style (task yields periodically) */ = SCHED_RR,
		FIFO /*! First in, first out (task won't yield until it calls Sched::yield(), sleeps or uses Sync IO) */ = SCHED_FIFO,
		OTHER = SCHED_OTHER
	};

	/*! \details This method will yield the process to another thread or process.  It is the
	 * same as calling sched_yield().
	 *
	 */
	static void yield(){ sched_yield(); }

	/*! \details Get the max priority for the specified policy */
	static int get_priority_max(enum policy value);
	/*! \details Get the min priority for the specified policy */
	static int get_priority_min(enum policy value);
	/*! \details Get the max priority for the specified policy */
	static int get_priority(pid_t pid);

	/*! \details Get the current proccess ID (equivalent to getpid()) */
	static pid_t get_pid(){ return getpid(); }

	/*! \details Get the RR interval for the pid
	 *
	 * @param pid  The process ID
	 * @return The RR interval in microseconds
	 */
	static int get_rr_interval(pid_t pid);

	/*! \details Set the scheduler
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
