/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SEM_HPP_
#define SEM_HPP_

#include <semaphore.h>
#include <fcntl.h>

namespace Sys {

/*! \brief Semaphore Class
 * \details This utilizes POSIX semaphores
 */
class Sem {
public:
	Sem();

	enum flags {
		CREATE = O_CREAT,
		EXCL = O_EXCL,
		EXCLUSIVE = O_EXCL
	};

	/*! \details Get the semaphore value
	 *
	 * @param sval A pointer to the destination to write the value
	 * @return Zero on success
	 */
	int getvalue(int *sval);

	/*! \details See getvalue() */
	inline int value(int *sval){ return getvalue(sval); }

	/*! \details Initialize an unnamed semaphore
	 *
	 * @param sem A pointer to the semaphore
	 * @param pshared Whether or not semaphore is shared between another process
	 * @param value The value of the semaphore
	 * @return Zero on success
	 */
	int init(sem_t * sem, int pshared, unsigned int value);

	/*! \details Destroy an unnamed semaphore (use with init() NOT open()) */
	int destroy();

	/*! \details This methods opens or creates a new semaphore
	 *
	 * @param name The name of the semaphore
	 * @param oflag Create/exclusivity
	 * @param mode Permissions
	 * @param value Initial value
	 * @return Zero on success
	 */
	int open(const char * name, int oflag, int mode, int value);

	/*! \details Close a named semaphore (use with open() NOT init()) */
	int close();

	/*! \details This will increment the semaphore value
	 * allowing another thread/process to use the semaphore.
	 *
	 * @return Zero on success
	 */
	int post();

	/*! \details Wait for semaphore to become available with a timeout */
	int timedwait(const struct timespec * timeout);
	/*! \details Check to see if semaphore is available */
	int trywait();
	/*! \details Wait for the semaphore to become available */
	int wait();
	/*! \details Delete a named semaphore */
	static int unlink(const char *name);

private:
	sem_t * handle;

};

};

#endif /* SEM_HPP_ */
