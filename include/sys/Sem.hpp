/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SEM_HPP_
#define SEM_HPP_

#ifndef __link


#include <semaphore.h>
#include <fcntl.h>

namespace sys {

/*! \brief Semaphore Class
 * \details This utilizes POSIX semaphores
 */
class Sem {
public:
	Sem();

	enum flags {
		CREATE /*! Create Semaphore flag */ = O_CREAT,
		EXCL /*! Create exclusive */ = O_EXCL,
		EXCLUSIVE /*! Create exclusive */ = O_EXCL
	};

	/*! \details Gets the semaphore value.
	 *
	 * @param sval A pointer to the destination to write the value
	 * @return Zero on success
	 */
	int get_value(int *sval);

	int value(int *sval){ return get_value(sval); }

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

	/*! \details This methods opens or creates a named semaphore
	 *
	 * @param name The name of the semaphore
	 * @param oflag Create/exclusivity
	 * @param mode Permissions
	 * @param value Initial value
	 * @return Zero on success
	 *
	 * \sa close()
	 */
	int open(const char * name, int oflag, int mode, int value);

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

	int timedwait(const struct timespec * timeout);

	/*! \details Waits for semaphore to become available with a timeout. */
	int wait_timed(const struct timespec * timeout){ return timedwait(timeout); }

	int trywait();
	/*! \details Checks to see if semaphore is available. */
	int try_wait(){ return trywait(); }

	/*! \details Waits for the semaphore to become available. */
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
