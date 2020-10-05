/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_SYS_THREAD_HPP
#define SAPI_SYS_THREAD_HPP

#include <pthread.h>

#include "../api/WorkObject.hpp"
#include "Sched.hpp"
#include "../api/SysObject.hpp"
#include "../chrono/Time.hpp"
#include "../chrono/MicroTime.hpp"

namespace sys {


/*! \brief Thread Class
 * \details This class creates and manages new threads using POSIX calls.
 *
 * Before creating the thread, the attributes should be set using:
 *
 * - set_stacksize()
 * - set_detachstate()
 *
 * Once the thread is running the scheduling policy can be updated using:
 *
 * - set_priority()
 *
 * ```
 *
 * static void thread_execute(void * args);
 *
 * Thread t; //default 4096 stack size and detached = true
 *
 * t.set_stacksize(2048);
 * t.set_detachstate(Thread::JOINABLE);
 *
 * //calling get_policy() or get_priority() before thread is created will return an error
 *
 * t.create(thread_execute);
 *
 * t.set_priority(5, Sched::RR); //priority 5 round robin scheduling
 *
 * //now that the thread is running, the stacksize and detachstate cannot be changed
 *
 * printf("Thread policy is %d\n", t.get_policy());
 * printf("Thread priority is %d\n", t.get_priority());
 * printf("Thread stacksize is %d\n", t.get_stacksize());
 * printf("Thread is joinable? %d\n", t.is_joinable());
 *
 * ```
 *
 *
 */
class Thread : public api::WorkObject {
public:

	enum detach_states {
		JOINABLE  = PTHREAD_CREATE_JOINABLE,
		DETACHED  = PTHREAD_CREATE_DETACHED,

		detach_state_joinable /*! Joinable thread */ = PTHREAD_CREATE_JOINABLE,
		detach_state_detached /*! Detacthed thread */ = PTHREAD_CREATE_DETACHED
	};


	class Options {
		API_ACCESS_FUNDAMENTAL(Options,u32,stack_size,4096);
		API_ACCESS_FUNDAMENTAL(Options,enum detach_states,detach_state,detach_state_detached);
	};

	typedef void *(*function_t)(void*);

	class CreateOptions {
		API_ACCESS_FUNDAMENTAL(CreateOptions,function_t,function,nullptr);
		API_ACCESS_FUNDAMENTAL(CreateOptions,void*,argument,nullptr);
		API_ACCESS_FUNDAMENTAL(CreateOptions,enum Sched::policy,policy,Sched::policy_other);
		API_ACCESS_FUNDAMENTAL(CreateOptions,int,priority,0);
	};

	using Id = arg::Argument<pthread_t, struct ThreadIdTag>;
	using StackSize = arg::Argument<u32, struct ThreadStackSizeTag>;
	using IsDetached = arg::Argument<bool, struct ThreadIsDetachedTag>;
	using Function = arg::Argument<void *(*)(void*), struct ThreadFunctionTag>;
	using FunctionArgument = arg::Argument<void*, struct ThreadFunctionArgumentTag>;
	using Return = arg::Argument<void**, struct ThreadReturnTag>;
	using DelayInterval = arg::Argument<const chrono::MicroTime&, struct ThreadDelayIntervalTag>;
	using Priority = Sched::Priority;

	/*! \details Defines the function call type that is
	 * used to create() a new thread.
	 *
	 */
	typedef void * (*handler_function_t)(void *);

	enum thread_flags {
		id_error /*! ID is an error */ = static_cast<u32>(-2),
		id_pending /*! ID is ready to be created (not valid yet) */ = static_cast<u32>(-1),
	};


	/*! \details Constructs a new thread object.
	 *
	 * @param stack_size  The stack size of the new thread (default is 4096)
	 * @param detached Whether to create as a detached thread.  If this value is false,
	 * another thread must use join() in order for the thread to terminate correctly.
	 */
	Thread(
			StackSize stacksize = StackSize(4096),
			IsDetached detached = IsDetached(true)
			);

	Thread(const Options & options);

	~Thread();

	/*! \details Sets the stacksize.
	  *
	  * @param size Stack size in bytes
	  *
	  * This method must be called before calling create().
	  *
	  *
	  */
	int set_stacksize(int size);

	/*! \details Gets the stacksize.
	  *
	  * This method will return zero if the stack size could not be
	  * retrieved. The error_number() method can be used to determine
	  * why.
	  *
	  */
	int get_stacksize() const;

	/*! \details Sets the detach state.
	  *
	  * @param value Detach state: use JOINABLE or DETACHED
	  *
	  * This method must be called before calling create().
	  *
	  *
	  */
	int set_detachstate(enum detach_states value);

	/*! \details Gets the detach state (Thread::JOINABLE or Thread::DETACHED). */
	int get_detachstate() const;

	/*! \details Sets the thread priority.
	  *
	  * @param prio Thread priority (higher value has higher priority)
	  * @param policy Scheduling policy
	  * @return Zero on success and less than zero for an error
	  *
	  * This method must be called after calling create().
	  *
	  */
	int set_priority(
			int prio,
			enum Sched::policy policy = Sched::RR);

	int set_priority(
			Priority prio,
			enum Sched::policy policy = Sched::RR){
		return set_priority(prio.argument(), policy);
	}

	/*! \details Gets the thread priority.
	  *
	  *
	  * Calling this method before the thread is created
	  * will return an error.
	  *
	  */
	int get_priority() const;

	/*! \details Get the thread policy.
	  *
	  * Calling this method before the thread is created
	  * will return an error.
	  *
	  */
	int get_policy() const;

	/*! \details Gets the ID of the thread. */
	pthread_t id() const { return m_id; }

	/*! \details Returns true if the thread has a valid id.
	  *
	  * If create() has not been called, this will return false.
	  * If there was an error creating the thread, this will
	  * also return false;
	  *
	  */
	bool is_valid() const;


	/*! \details Starts the thread.
	 *
	 * @param func The function to execute as a new thread
	 * @param args The arguments to pass to the new thread
	 * @param prio The priority to set for the new thread
	 * @param policy The new thread's scheduling policy
	 * @return Zero on success
	 *
	 * This method creates a new thread. The Thread object only manages one thread
	 * at a time. To create multiple threads, you will need multiple instances of the
	 * Thread object.
	  *
	  *
	  * Use the following methods to change thread attributes:
	  *
	  * - set_stacksize(): must be called before calling this method
	  * - set_detachstate(): must be called before calling this method
	  * - set_priority(): must be called after calling this method
	  *
	 *
	 */
	int create(const CreateOptions & options);

	int create(
			Function func,
			FunctionArgument args = FunctionArgument(nullptr),
			Priority prio = Priority(0),
			enum Sched::policy policy = Sched::OTHER
			){
		return create(CreateOptions()
									.set_function(func.argument())
									.set_argument(args.argument())
									.set_policy(policy)
									.set_priority(prio.argument())
									);
	}

	enum cancel_types {
		cancel_type_deferred = PTHREAD_CANCEL_DEFERRED,
		cancel_type_asynchronous = PTHREAD_CANCEL_ASYNCHRONOUS
	};

	static int set_cancel_type(enum cancel_types cancel_type);

	enum cancel_states {
		cancel_state_enable = PTHREAD_CANCEL_ENABLE,
		cancel_state_disable = PTHREAD_CANCEL_DISABLE
	};

	static int set_cancel_state(enum cancel_states cancel_state);

	int cancel() const;

	/*! \details Checks if the thread is running.
	 *
	 * If the thread is no longer running, the thread id
	 * value is reset to its initial state.
	 *
	 * This MUST be called on detached threads after
	 * the thread has finished and before another
	 * thread is created.
	 *
	 * This is also called by `wait()`. So that works too.
	 *
	 */
	bool is_running();

	/*! \details Waits for the thread to complete (joins thread if it is not detached).
	 *
	 * If the thread is joinable, this method will join the thread which blocks until
	 * the thread returns. If the thread is not-joinable, this method will poll
	 * is_running() until the thread completes.
	 *
	 */
	int wait(
			Return ret = Return(nullptr),
			DelayInterval interval = DelayInterval(chrono::Milliseconds(1000UL))
			);

	/*! \details Yields the processor to another thread */
	static void yield(){ return Sched::yield(); }

	/*! \details Joins the calling thread to the specified thread.
	  *
	  * @param thread A reference to the thread to joing
	  * @param value_ptr a pointer to where the return value of \a thread will be stored
	  * @return Zero on success
	  *
	  * This method will block the calling thread until the joined
	  * thread returns.
	  *
	  */
	static int join(
			Thread & thread_to_join,
			void ** result = nullptr
			);

	static int join(
			Thread & thread_to_join,
			Return return_value = Return(nullptr)
			){
		return join(thread_to_join, return_value.argument());
	}

	/*! \details Returns the thread ID of the calling thread. */
	static pthread_t self(){ return pthread_self(); }

	/*! \details Returns the process ID of the calling thread. */
	static pid_t get_pid(){ return Sched::get_pid(); }


	/*! \details Sends a signal to the thread.
	 *
	 * @param sig The signal to send
	 * @return Zero on success
	  *
	  * This method sends a signal to the thread (equivalend to pthread_kill()).
	  * It causes the signal handler to be executed in the context of the thread,
	  * but the signal action will affect the whole process.
	  *
	  * ```
	  *
	  * Thread t;
	  *
	  * //create the thread
	  *
	  * t.kill(0); //returns zero if the thread is valid but doesn't send a signal
	  * t.kill(Signal::KILL); //will cause the entire process to die
	  * t.kill(Signal::STOP); //will stop the thread from running
	  *
	  *
	  * ```
	  *
	  *
	 */
	int kill(int signal_number){
		return pthread_kill(m_id, signal_number);
	}


	/*! \details This method returns true if the thread is joinable */
	bool is_joinable() const;

	/*! \details Joins the calling thread to this object's thread.
	 *
	  * @param value_ptr A pointer to where the return value of the thread function will be stored (ignored if null)
	 * @return 0 if joined, -1 if couldn't join (doesn't exist or is detached)
	  *
	  * This method will block the calling thread until the thread function
	  * returns.
	 */
	int join(void ** result);
	int join(Return value_ptr = Return(nullptr)){
		return join(value_ptr.argument());
	}

	/*! \details Allows read only access to the thread attributes. */
	const pthread_attr_t & attr() const { return m_pthread_attr; }

private:
	pthread_attr_t m_pthread_attr;

#if defined __link
	u32 m_status;
	pthread_t m_id = {0};
#else
	pthread_t m_id = 0;
#endif

	int init(int stack_size, bool detached);
	int reset();



	void set_id_pending(){
#if defined __link
		m_status = id_pending;
#else
		m_id = id_pending;
#endif
	}
	void set_id_error(){
#if defined __link
		m_status = id_error;
#else
		m_id = id_error;
#endif
	}

	bool is_id_pending() const {
#if defined __link
		return m_status == id_pending;
#else
		return m_id == id_pending;
#endif
	}
	bool is_id_error() const {
#if defined __link
		return m_status == id_error;
#else
		return m_id == id_error;
#endif
	}
};

}

#endif /* SAPI_SYS_THREAD_HPP */
