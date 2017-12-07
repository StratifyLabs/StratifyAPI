/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SEM_HPP_
#define SEM_HPP_

#ifndef __link


#include <semaphore.h>
#include <fcntl.h>

namespace sys {

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
 * sem.wait(); //decrement the semaphore value, block if semaphore can't be decremented
 * sem.post(); //increment semaphore value
 * sem.close(); //close reference to semaphore
 * sem.unlink(); //delete semaphore if all references are closed
 * \endcode
 *
 */
class Sem {
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
	int init(sem_t * sem, int pshared, unsigned int value);

	/*! \details Destroys an unnamed semaphore.
	 *
	 * This method is used in conjuntion with init() not open().
	 *
	 */
	int destroy();

	/*! \details Opens a named semaphore
	 *
	 * @param name The name of the semaphore
	 * @return Zero on success
	 *
	 * \sa close()
	 */
	int open(const char * name, int o_flags = 0, int mode = 0, int value = 0);

	/*! \details Creates a new named semaphore.
	 *
	 * @param name The name of the semaphore
	 * @param value The value assigned to the semaphore
	 * @param exclusive If true, the method will fail if the semaphore already exists. If false, it will open the existing semaphore.
	 * @return Zero on success or less than zero with errno set
	 *
	 *
	 *
	 */
	int create(const char * name, int value, bool exclusive = false);

	/*! \details Closes a named semaphore.
	 *
	 * \sa open()
	 */
	int close();

	/*! \details Increments the semaphore value
	 * allowing another thread/process to use the semaphore.
	 *
	 * @return Zero on success
	 */
	int post();


	/*! \details Executes a timed wait for the semaphore.
	 *
	 * @param timeout The amount of time to wait before timing out
	 * @return Zero on success, less than zero for an error or timeout
	 *
	 * This method will block until either the semaphore becomes available
	 * and is decremented or until the timeout is exceeded.
	 *
	 */
	int wait_timed(const struct timespec & timeout);
	int timedwait(const struct timespec & timeout){ return wait_timed(timeout); }

	/*! \details Checks to see if semaphore is available. */
	int try_wait();

	/*! \details Waits for the semaphore to become available.
	 *
	 * This method will block until the semaphore becomes available and decrement
	 * the semaphore value. The post() method will release (increment the value of) the semaphore.
	 */
	int wait();

	/*! \details Deletes a named semaphore.
	 *
	 * @param name The name of the semaphore
	 *
	 */
	static int unlink(const char *name);

private:
	sem_t * m_handle;

};

};

#endif

#endif /* SEM_HPP_ */
