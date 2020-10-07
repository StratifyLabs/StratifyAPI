/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */

#ifndef SAPI_SYS_MUTEX_HPP_
#define SAPI_SYS_MUTEX_HPP_

#include "api/api.hpp"

#include "chrono/Time.hpp"

#include <mcu/types.h>
#include <pthread.h>

#if defined __android
#define PTHREAD_PRIO_PROTECT PTHREAD_PRIO_NONE
#endif

namespace thread {

/*! \brief Mutex Class
 * \details This class holds a Mutex used with
 * multi-threaded applications.
 *
 */
class Mutex : public api::Object {
public:
  /*! \details Mutex Types */
  enum class Type {
    normal /*! Normal Mutex Type */ = PTHREAD_MUTEX_NORMAL,
    recursive /*! Recursive Mutex tyep */ = PTHREAD_MUTEX_RECURSIVE
  };

  /*! \details Mutex Protocol values */
  enum class Protocol {
    prio_none = PTHREAD_PRIO_NONE,
    prio_inherit = PTHREAD_PRIO_INHERIT,
    prio_protect = PTHREAD_PRIO_PROTECT,
  };

  class Attributes : public api::Object {
  public:
    /*! \details Constructs a Mutex attributes object with default values. */
    Attributes();

    /*! \details Constructs a Mutex attributes object using the specified
     * attributes. */
    explicit Attributes(const pthread_mutexattr_t &mutexattr)
      : m_item(mutexattr) {}

    ~Attributes();

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
    Attributes &set_prio_ceiling(int ceiling);

    /*! \details Sets the protocol. */
    Attributes &set_protocol(Protocol value);

    /*! \details Sets whether this is shared between processes. */
    Attributes &set_pshared(bool value = true);

    /*! \details Set the mutex type. */
    Attributes &set_type(Type value);

    /*! \details Returns the priority ceiling. */
    int get_prio_ceiling() const;

    /*! \details Returns the protocol. */
    int get_protocol() const;

    /*! \details Returns the type. */
    int get_type() const;

    /*! \details Returns true if the mutex attributes are for process sharing.
     */
    bool get_pshared() const;

  private:
    friend class Mutex;
    void invalidate() {
#if defined __link
      m_is_valid = false;
#else
      m_item.is_initialized = 0;
#endif
    }

    void validate() {
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

  /*! \details Constructs a Mutex with the default settings. */
  Mutex();

  /*! \details Constructs a Mutex using the specified attributes. */
  Mutex(const Attributes &attr);

  /*! \details Sets the mutex attributes. */
  Mutex &set_attributes(const Attributes &attr);

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
  Mutex &lock();

  /*! \details Attempts to lock the mutex but times out after the specified
   * duration.
   *
   * @param clock_time A clock time object that defines the maximum amount of
   * time to wait for a lock
   *
   */
#if !defined __link
  Mutex &lock_timed(const chrono::ClockTime &clock_time);
#endif

  /*! \details Attempts to lock the mutex.
   *
   * If the mutex is
   * locked by another thread, the method returns immediately, returns less than
   * zero, and sets errno to EBUSY.
   *
   * If the mutex is locked by the current thread, it will be incremented if it
   * is recursive or just return zero and keep the lock if non-recursive.
   *
   */
  Mutex &try_lock();

  /*! \details Unlocks the mutex. */
  Mutex &unlock();

private:
  pthread_mutex_t m_item;
};

class MutexGuard {
public:
  explicit MutexGuard(Mutex &mutex) : m_mutex(mutex) { mutex.lock(); }
  ~MutexGuard() { m_mutex.unlock(); }

private:
  Mutex &m_mutex;
};

} // namespace thread

#endif /* SAPI_SYS_MUTEX_HPP_ */