/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef PHY_PERIPH_HPP_
#define PHY_PERIPH_HPP_

#include <iface/link.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <stratify/stratify.h>

#ifndef __link
#include "../Sys/Aio.hpp"
#else
#undef fileno
#endif

#include "Phy.hpp"

namespace Hal {


/*! \brief Peripheral Class
 * \details This is an abstract class for a microcontroller peripheral.
 *
 * All peripherals have a common interface based on the following function:
 * - open()
 * - close()
 * - read()
 * - write()
 * - ioctl()
 *
 * The classes that inherit Periph implement a method for each ioctl() call available
 * on the peripheral.  For example, the UART has an ioctl request called I_UART_SETATTR so
 * the method Uart::setattr() implements the ioctl request.
 *
 */
class Periph : public Phy {
public:
	typedef unsigned int port_t;
	Periph(core_periph_t periph, port_t port);

	/*!  \details This method opens the peripheral.  For each instance, the peripheral
	 * only needs to be opened one time.  The port is typically opened with open() and
	 * configured with setattr().  After that, other instances of the peripheral can read
	 * and write without opening again.
	 */
	int open(int flags = READWRITE);

	/*! \details reads the device. */
	int read(void * buf, int nbyte);

	/*! \details reads the device at the location */
	int read(int loc, void * buf, int nbyte);

	/*! \details writes the device at the location
	 *
	 * @param loc Location to read (not application to character devices)
	 * @param buf Pointer to the source data
	 * @param nbyte Number of bytes to write
	 * @return Number of bytes successfully written or -1 with errno set
	 */
	int write(int loc, const void * buf, int nbyte);

	/*! \details reads a line from the device */
	int readline(char * buf, int nbyte, int timeout = 10, char term = '\n');

	/*! \details writes data to the device */
	int write(const void * buf, int nbyte);

	/*! \details writes a string to the device */
	int write(const char * s){ return write(s, strlen(s)); }

#ifndef __MCU_ONLY__
	/*! \details This method closes the peripheral.  Once a single instance closes the peripheral,
	 * no other instances will be able to access it.
	 */
	int close();

#else
	int close() = 0;
#endif



#ifndef __link
	/*! \brief Asynchronous read */
	virtual int read(Sys::Aio & aio);
	/*! \brief Asynchronous write */
	virtual int write(Sys::Aio & aio);
#endif


#ifndef __MCU_ONLY__
	/*! \brief Return the file descriptor for the peripheral */
	int fileno(void);
	using Phy::ioctl;
	int ioctl(int req, void * arg);
	int seek(int loc, int whence = SET);

#endif

	port_t port() const{ return periph_port & 0xFF; }

protected:
	u16 periph_port;
#ifndef __MCU_ONLY__
	int fd;
#else
	int flags;
#endif


private:
#if !defined __link
	static u16 fd_map[OPEN_MAX];

	void update_fileno();
	int lookup_fileno();
	int open(const char * name, int flags);

#endif



};

};

#endif /* PHY_PERIPH_HPP_ */
