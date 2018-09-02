/* Copyright 2016 tgil All Rights Reserved */

#ifndef SYS_MUTEX_HPP_
#define SYS_MUTEX_HPP_


#include "../api/SysObject.hpp"
#include "../chrono/MicroTime.hpp"
#include <pthread.h>
#include <mcu/types.h>

namespace sys {

/*! \brief Mutex Attibutes
 * \details The MutexAttr class manages
 * a pthread_mutexattr_t object.
 *
 *
 */
class MutexAttr : public api::SysInfoObject {
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


    /*! \details Constructs a Mutex attributes object with default values. */
    MutexAttr();

    /*! \details Constructs a Mutex attributes object using the specified attributes. */
    MutexAttr(const pthread_mutexattr_t & mutexattr){
        m_item = mutexattr;
    }

    ~MutexAttr();

    /*! \details Constructs a mutex attributs object with the specified values.
     *
     * @param t Mutex type (NORMAL or RECURSIVE)
     * @param prio_ceiling The priority ceiling (0 is default, lowest priority)
     * @param pshared True if mutex is to be shared between processes
     *
     */
    MutexAttr(enum type t, int prio_ceiling = 0, bool pshared = false);

    /*! \details Sets the priority ceiling.
     *
     * @param ceiling The priority ceiling of the mutex
     * @return Zero on success
     *
     */
    int set_prio_ceiling(int ceiling);

    /*! \details Sets the protocol. */
    int set_protocol(enum protocol value);

    /*! \details Sets whether this is shared between processes. */
    int set_pshared(bool value = true);

    /*! \details Set the mutex type (NORMAL_TYPE or RECURSIVE_TYPE) */
    int set_type(enum type value);

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
    pthread_mutexattr_t m_item;
};


/*! \brief Mutex Class
 * \details This class holds a Mutex used with
 * multi-threaded applications.
 *
 */
class Mutex : public api::SysWorkObject {
public:
    /*! \details Constructs a Mutex with the default settings. */
    Mutex();

    /*! \details Constructs a Mutex using the specified attributes. */
    Mutex(const MutexAttr & attr);

    /*! \details Sets the mutex attributes. */
    int set_attr(const MutexAttr & attr);

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

#endif /* SYS_MUTEX_HPP_ */
