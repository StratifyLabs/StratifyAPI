/* Copyright 2016 tgil All Rights Reserved */

#ifndef SYS_MUTEX_HPP_
#define SYS_MUTEX_HPP_

#ifndef __link

#include <pthread.h>
#include <mcu/types.h>

namespace sys {

class MutexAttr {
public:

	/*! \details Mutex Types */
	enum type {
		NORMAL /*! Normal Mutex Type */ = PTHREAD_MUTEX_NORMAL,
		RECURSIVE /*! Recursive Mutex tyep */ = PTHREAD_MUTEX_RECURSIVE
	};

	/*! \details Mutex Protocol values */
	enum protocol {
		PRIO_NONE_PROTOCOL = PTHREAD_PRIO_NONE,
		PRIO_INHERIT_PROTOCOL = PTHREAD_PRIO_INHERIT,
		PRIO_PROTECT_PROTOCOL = PTHREAD_PRIO_PROTECT,
	};


	/*! \details Construct a Mutex with default values */
	MutexAttr();
	~MutexAttr();
	/*! \details Construct a Mutex with the specified values */
	MutexAttr(enum type t, int prio_ceiling = 0, bool pshared = false);


	/*! \details Set the priority ceiling */
	int set_prio_ceiling(int value);
	/*! \details Set the protocol */
	int set_protocol(enum protocol value);
	/*! \details Set whether this is shared between processes */
	int set_pshared(bool value = true);

	/*! \details Set the mutex type (NORMAL_TYPE or RECURSIVE_TYPE) */
	int set_type(enum type value);

	int get_prio_ceiling() const;
	int get_protocol() const;
	int get_type() const;
	bool get_pshared() const;

	const pthread_mutexattr_t & value() const { return m_item; }

private:
	pthread_mutexattr_t m_item;
};


/*! \brief Mutex Class
 * \details This class holds a Mutex used with
 * multi-threaded applications.
 *
 */
class Mutex {
public:
	/*! \details Construct a Mutex with the default settings */
	Mutex();

	/*! \details Construct a Mutex using the specified attributes */
	Mutex(const MutexAttr & attr);

	/*! \details This method attempts to lock the mutex.  If the mutex is locked by
	 * another thread, the thread will block until the mutex becomes available.
	 *
	 * If the thread already holds a lock on the mutex, it will
	 *
	 * - increment the count if the mutex is recursive, or
	 * - return less than zero and set errno to EDEADLK
	 */
	int lock();

	/*! \details Try to lock the mutex but force a timeout
	 * after the specified duration
	 *
	 * @param sec The number of seconds to wait before a timeout
	 * @param usec The number of microseconds to wait for a lock before a timeout
	 *
	 */
	int lock_timed(u32 sec, u32 usec = 0);

	/*! \details This methods attempts to lock the mutex.  If the mutex is
	 * locked by another thread, the method returns immediately, returns less than zero,
	 * and sets errno to EBUSY.
	 *
	 * If the mutex is locked by the current thread, it will be incremented if it is recursive or just
	 * return zero and keep the lock if non-recursive.
	 *
	 */
	int try_lock();

	/*! \details Unlock the mutex */
	int unlock();

private:
	pthread_mutex_t m_item;
};


}
#endif //__link

#endif /* SYS_MUTEX_HPP_ */
