/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef THREADAPI_THREAD_SEM_HPP_
#define THREADAPI_THREAD_SEM_HPP_

#ifndef __link

#include <fcntl.h>
#include <semaphore.h>

#include "chrono/ClockTime.hpp"

namespace thread {

/*! \brief Semaphore Class
 * \details This class is a wrapper for POSIX style semphores.
 *
 * Here is an example using named semaphores.
 *
 * \code
 *
 * #include <sapi/sys.hpp>
 *
 * Sem sem;
 * sem.create("my_semaphore", 10);
 * sem.wait(); //decrement the semaphore value, block if semaphore can't be
 * decremented sem.post(); //increment semaphore value sem.close(); //close
 * reference to semaphore sem.unlink(); //delete semaphore if all references are
 * closed \endcode
 *
 */
class Sem : public api::Object {
public:
  Sem();

  enum {
    CREATE /*! Create Semaphore flag */ = O_CREAT,
    EXCL /*! Create exclusive */ = O_EXCL,
    EXCLUSIVE /*! Create exclusive */ = O_EXCL
  };

  /*! \details Gets the semaphore value.
   *
   * @return The value of the semaphore (>= 0) or -1 on an error
   */
  int get_value() const;

  /*! \details Initializes an unnamed semaphore.
   *
   * @param sem A pointer to the semaphore
   * @param pshared Whether or not semaphore is shared between another process
   * @param value The value of the semaphore
   * @return Zero on success
   *
   * \sa destroy()
   *
   */
  Sem &initialize(sem_t *sem, int pshared, unsigned int value);

  /*! \details Destroys an unnamed semaphore.
   *
   * This method is used in conjuntion with initialize() not open().
   *
   */
  Sem &finalize();

  class Open {
    API_AF(Open, int, o_flags, 0);
    API_AF(Open, int, mode, 0);
    API_AF(Open, int, value, 0);
  };

  /*! \details Opens a named semaphore
   *
   * @param name The name of the semaphore
   * @param o_flags Flags used to open semaphore
   * @param mode The access mode permissions for the semaphore
   * @param value The value of the semaphore (used when creating)
   * @return Zero on success
   *
   * \sa close()
   */
  Sem &open(var::StringView name, const Open &options);

  enum class Exclusive { no, yes };

  /*! \details Creates a new named semaphore.
   *
   * @param name The name of the semaphore
   * @param value The value assigned to the semaphore
   * @param exclusive If true, the method will fail if the semaphore already
   * exists. If false, it will open the existing semaphore.
   * @return Zero on success or less than zero with errno set
   *
   *
   *
   */
  Sem &create(var::StringView name, int value, Exclusive exclusive);

  /*! \details Closes a named semaphore.
   *
   * \sa open()
   */
  Sem &close();

  /*! \details Increments the semaphore value
   * allowing another thread/process to use the semaphore.
   *
   * @return Zero on success
   */
  Sem &post();

  /*! \details Executes a timed wait for the semaphore.
   *
   * @param clock_time The amount of time to wait before timing out
   * @return Zero on success, less than zero for an error or timeout
   *
   * This method will block until either the semaphore becomes available
   * and is decremented or until the timeout is exceeded.
   *
   */
  Sem &wait_timed(const chrono::ClockTime &clock_time);

  /*! \details Equivalent to wait_timed() but follow the posix naming. */
  Sem &timedwait(const chrono::ClockTime &clock_time) {
    return wait_timed(clock_time);
  }

  /*! \details Checks to see if semaphore is available. */
  Sem &try_wait();

  /*! \details Waits for the semaphore to become available.
   *
   * This method will block until the semaphore becomes available and decrement
   * the semaphore value. The post() method will release (increment the value
   * of) the semaphore.
   */
  Sem &wait();

  /*! \details Deletes a named semaphore.
   *
   * @param name The name of the semaphore
   *
   */
  Sem &unlink(var::StringView name);

private:
  sem_t *m_handle = nullptr;
};

} // namespace thread

#endif

#endif /* THREADAPI_THREAD_SEM_HPP_ */
