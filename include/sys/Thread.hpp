/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef THREAD_HPP_
#define THREAD_HPP_



#ifndef __link

#include "../api/WorkObject.hpp"
#include <mcu/types.h>
#include <pthread.h>
#include "Sched.hpp"
#include "../api/SysObject.hpp"

namespace sys {

/*! \brief Thread Class
 * \details This class creates and manages new threads using POSIX calls.
 *
 */
class Thread : public api::SysWorkObject {
public:

    typedef void * (*handler_function_t)(void *);

	enum {
		ID_ERROR /*! ID is an error */ = -2,
        ID_PENDING /*! ID is ready to be created (not valid yet) */ = -1,
		JOINABLE /*! Joinable thread */ = PTHREAD_CREATE_JOINABLE,
		DETACHED /*! Detacthed thread */ = PTHREAD_CREATE_DETACHED
	};

	/*! \details Constructs a new thread object.
	 *
	 * @param stack_size  The stack size of the new thread (default is 4096)
	 * @param detached Whether to create as a detached thread.  If this value is false,
	 * another thread must use join() in order for the thread to terminate correctly.
	 */
	Thread(int stack_size = 4096, bool detached = true);

	~Thread();

	/*! \details Sets the stacksize (no effect after create() method). */
	int set_stacksize(int size);

	/*! \details Gets the stacksize. */
	int get_stacksize() const;

    /*! \details Sets the detach state.
     *
     * @param value Detach state: use JOINABLE or DETACHED
     */
    int set_detachstate(int value);

	/*! \details Gets the detach state (Thread::JOINABLE or Thread::DETACHED). */
	int get_detachstate() const;

	/*! \details Sets the thread priority. */
	int set_priority(int prio, enum Sched::policy policy = Sched::RR);

	/*! \details Gets the thread priority. */
	int get_priority() const;

	/*! \details Get the thread policy. */
	int get_policy() const;

	/*! \details Gets the ID of the thread. */
	int id() const { return m_id; }

    /*! \details Returns true if the thread has a valid id.
     *
     * If create() has not been called, this will return false.
     * If there was an error creating the thread, this will
     * also return false;
     *
     */
    bool is_valid() const { return m_id > 0; }


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
	 */
    int create(handler_function_t func, void * args = NULL, int prio = 0, enum Sched::policy policy = Sched::OTHER);

	/*! \details Checks if the thread is running. */
	bool is_running() const;

	/*! \details Waits for the thread to complete (joins thread if it is not detached).
	 *
	 * If the thread is joinable, this method will join the thread which blocks until
	 * the thread returns. If the thread is not-joinable, this method will poll
	 * is_running() until the thread completes.
	 *
	 */
    int wait(void ** ret = 0, int interval = 1000);

	/*! \details Yields the processor to another thread */
    static void yield(){ return Sched::yield(); }

    /*! \details Joins the calling thread to the specified thread.
	 *
	 * @param ident the ID of the target thread
	 * @param value_ptr A pointer to the return value of the target thread
	 * @return Zero on success
     *
     * This method will block the calling thread until the joined
     * thread returns.
     *
     *
	 */
	static int join(int ident, void ** value_ptr = 0);

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
    static int join(const Thread & thread, void ** value_ptr = 0){
        return join(thread.id(), value_ptr);
    }

    /*! \details Returns the thread ID of the calling thread. */
    static pthread_t self(){ return pthread_self(); }

    /*! \details Returns the process ID of the calling thread. */
    static pid_t get_pid(){ return Sched::get_pid(); }


	/*! \details Sends a signal to the thread.
	 *
	 * @param sig The signal to send
	 * @return Zero on success
	 */
	int kill(int sig){ return pthread_kill(m_id, sig); }


	/*! \details This method returns true if the thread is joinable */
	bool is_joinable() const;

    /*! \details Joins the calling thread this object's thread.
	 *
     * @param value_ptr A pointer to where the return value of the thread function will be stored (ignored if null)
	 * @return 0 if joined, -1 if couldn't join (doesn't exist or is detached)
     *
     * This method will block the calling thread until the thread function
     * returns.
	 */
	int join(void ** value_ptr = 0) const;

	/*! \details Allows read only access to the thread attributes. */
	const pthread_attr_t & attr() const { return m_pthread_attr; }

private:
	pthread_attr_t m_pthread_attr;
	pthread_t m_id;

	int init(int stack_size, bool detached);
    int reset();

    void set_id_pending(){ m_id = ID_PENDING; }
	void set_id_error(){ m_id = ID_ERROR; }
};

}

#endif

#endif /* THREAD_HPP_ */
