/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */

#ifndef SAPI_SYS_MUTEX_HPP_
#define SAPI_SYS_MUTEX_HPP_


#include "../api/SysObject.hpp"
#include "../chrono/MicroTime.hpp"
#include <pthread.h>
#include <mcu/types.h>

namespace sys {

class MutexFlags {
public:

	/*! \details Mutex Types */
	enum type {
		/*! \cond */
		NORMAL /*! Normal Mutex Type */ = PTHREAD_MUTEX_NORMAL,
		RECURSIVE /*! Recursive Mutex tyep */ = PTHREAD_MUTEX_RECURSIVE,
		/*! \endcond */
		normal /*! Normal Mutex Type */ = PTHREAD_MUTEX_NORMAL,
		recursive /*! Recursive Mutex tyep */ = PTHREAD_MUTEX_RECURSIVE
	};

	/*! \details Mutex Protocol values */
	enum protocol {
		/*! \cond */
		PRIO_NONE_PROTOCOL = PTHREAD_PRIO_NONE,
		PRIO_INHERIT_PROTOCOL = PTHREAD_PRIO_INHERIT,
		PRIO_PROTECT_PROTOCOL = PTHREAD_PRIO_PROTECT,
		/*! \endcond */
		prio_none_protocol = PTHREAD_PRIO_NONE,
		prio_inherit_protocol = PTHREAD_PRIO_INHERIT,
		prio_protect_protocol = PTHREAD_PRIO_PROTECT,
	};

};

/*! \brief Mutex Attibutes
 * \details The MutexAttr class manages
 * a pthread_mutexattr_t object.
 *
 *
 */
class MutexAttributes : public MutexFlags {
public:

	/*! \details Constructs a Mutex attributes object with default values. */
	MutexAttributes();

	/*! \details Constructs a Mutex attributes object using the specified attributes. */
	MutexAttributes(const pthread_mutexattr_t & mutexattr){
		m_item = mutexattr;
	}

	~MutexAttributes();

	bool is_valid() const {
#if defined __link
		return m_is_valid;
#else
		return m_item.is_initialized != 0;
#endif
	}

	/*! \details Sets the priority ceiling.
	  *
	  * @param ceiling The priority ceiling of the mutex
	  * @return Zero on success
	  *
	  */
	MutexAttributes & set_prio_ceiling(int ceiling);

	/*! \details Sets the protocol. */
	MutexAttributes & set_protocol(enum protocol value);

	/*! \details Sets whether this is shared between processes. */
	MutexAttributes & set_pshared(bool value = true);

	/*! \details Set the mutex type. */
	MutexAttributes & set_type(enum type value);

	/*! \details Returns the priority ceiling. */
	int get_prio_ceiling() const;

	/*! \details Returns the protocol. */
	int get_protocol() const;

	/*! \details Returns the type. */
	int get_type() const;

	/*! \details Returns true if the mutex attributes are for process sharing. */
	bool get_pshared() const;

private:
	friend class Mutex;
	void invalidate(){
#if defined __link
		m_is_valid = false;
#else
		m_item.is_initialized = 0;
#endif
	}

	void validate(){
#if defined __link
		m_is_valid = true;
#else
#endif
	}
	pthread_mutexattr_t m_item;
#if defined __link
	bool m_is_valid;
#endif
};

typedef MutexAttributes MutexAttr;


/*! \brief Mutex Class
 * \details This class holds a Mutex used with
 * multi-threaded applications.
 *
 */
class Mutex : public api::WorkObject, public MutexFlags {
public:
	/*! \details Constructs a Mutex with the default settings. */
	Mutex();

	/*! \details Constructs a Mutex using the specified attributes. */
	Mutex(const MutexAttributes & attr);

	/*! \details Sets the mutex attributes. */
	int set_attributes(const MutexAttributes & attr);
	Mutex & operator << (const MutexAttributes & a){
		set_attributes(a);
		return *this;
	}

	/*! \details Attempts to lock the mutex.
	  *
	  * If the mutex is locked by
	  * another thread, the thread will block until the mutex becomes available.
	  *
	  * If the thread already holds a lock on the mutex, it will
	  *
	  * - increment the count if the mutex is recursive, or
	  * - return less than zero and set errno to EDEADLK
	  */
	int lock();

	/*! \details Attempts to lock the mutex but times out after the specified duration.
	  *
	  * @param clock_time A clock time object that defines the maximum amount of time to wait for a lock
	  *
	  */
#if !defined __link
	int lock_timed(const chrono::ClockTime & clock_time);
#endif

	/*! \details Attempts to lock the mutex.
	  *
	  * If the mutex is
	  * locked by another thread, the method returns immediately, returns less than zero,
	  * and sets errno to EBUSY.
	  *
	  * If the mutex is locked by the current thread, it will be incremented if it is recursive or just
	  * return zero and keep the lock if non-recursive.
	  *
	  */
	int try_lock();

	/*! \details Unlocks the mutex. */
	int unlock();

private:
	pthread_mutex_t m_item;
};


}

#endif /* SAPI_SYS_MUTEX_HPP_ */
