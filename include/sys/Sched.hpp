/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#ifndef SAPI_SYS_SCHED_HPP_
#define SAPI_SYS_SCHED_HPP_

#include "../api/SysObject.hpp"
#include "../arg/Argument.hpp"

#if defined __win32
typedef int pid_t;
#endif

namespace sys {

/*! \brief Sched Class
 * \details The Sched class is used
 * to change the scheduing policy of the current
 * thread or process.
 *
 * Stratify OS has three scheduling policies:
 *
 * - Other
 * - Round Robin
 * - FIFO
 *
 * Other (Sched::OTHER) is the default. It is a round robin scheduler with a single priority (lowest priority).
 * When a thread or process is using Sched::OTHER, it will yield the CPU in the following cases:
 *
 * - It's round robin time is expired and it will be pre-empted
 * - It makes a call to a blocking function:
 *   - usleep() (chrono::Timer::wait_microseconds())
 *   - sleep() (chrono::Timer::wait_seconds())
 *   - read() or write() (not all read or writes will yield the CPU, only those that can be accomplished in the background)
 *   - sched_yield() (Sched::yield())
 *   - mutexes (Sys::Mutex), semaphores (Sys::Sem), message queues (Sys::Mq), and other system sync mechanisms can also cause the thread to block and yield the processor
 * - It receives a signal that pauses the process (SIGSTOP for example)
 *
 * Round Robin (Sched::RR) scheduling behaves the same as Sched::OTHER but allows different priorities to be set. Lower
 * priority threads will ONLY get CPU time if higher priority threads are blocked. The thread will block in the same cases
 * as Sched::OTHER.
 *
 * FIFO (Sched::FIFO) scheduling will not be pre-empted by the round robin timer. It will keep control of the CPU
 * until it calls a function had yields (same as the functions for Sched::OTHER). FIFO scheduling is good for
 * high priority data processing (such as handling a packet arriving from the network).
 *
 * None of the scheduling modes will meet hard real-time constraints. In those cases, it is nessary to write a kernel
 * driver or request (Sys::request()) (integrated with the Board support package) that can handle real-time constraints in
 * interrupt services routines with very low latency.
 *
 *
 *
 */
class Sched : public api::InfoObject {
public:

	using ProcessId = arg::Argument<u32, struct SchedProcessIdTag>;
	using Priority = arg::Argument<u32, struct SchedPriorityTag>;

	enum policy {
#if defined __win32
		RR, FIFO, OTHER
#else
		RR /*! Round Robin style (task yields periodically) */ = SCHED_RR,
		FIFO /*! First in, first out (task won't yield until it calls Sched::yield(), sleeps or uses Sync IO) */ = SCHED_FIFO,
		OTHER /*! Default scheduling: round robin with no priority) */ = SCHED_OTHER
#endif
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
	static int get_priority(ProcessId pid){ return get_priority(pid.argument()); }

	/*! \details Gets the current proccess ID (equivalent to getpid()). */
	static pid_t get_pid(){ return getpid(); }

	/*! \details Gets the RR interval for the pid.
	 *
	 * @param pid  The process ID
	 * @return The RR interval in microseconds
	 */
	static int get_rr_interval(pid_t pid);
	static int get_rr_interval(ProcessId pid){ return get_rr_interval(pid.argument()); }

	/*! \details Sets the scheduler with the given parameters.
	 *
	 * @param pid The process ID
	 * @param value The polic (such as Sched::FIFO)
	 * @param priority The priority (higher is higher priority)
	 * @return Zero on success of -1 with errno set
	 */

	static int set_scheduler(
			pid_t pid,
			enum policy value,
			Priority priority
			);

	static int set_scheduler(
			ProcessId pid,
			enum policy value,
			Priority priority
			){
		return set_scheduler(pid.argument(), value, priority);
	}



};

} /* namespace sys */



#endif /* SAPI_SYS_SCHED_HPP_ */
