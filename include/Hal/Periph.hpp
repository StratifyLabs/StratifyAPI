/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef PHY_PERIPH_HPP_
#define PHY_PERIPH_HPP_

#include <iface/link.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <stratify/stratify.h>

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
	 * configured with set_attr().  After that, other instances of the peripheral can read
	 * and write without opening again.
	 */
	int open(int flags = READWRITE);
	int ioctl(int req, void * arg) const;
	int seek(int loc, int whence) const;
	int fileno(void) const;
	int read(void * buf, int nbyte) const;
	int write(const void * buf, int nbyte) const;
#ifndef __link
	int read(Sys::Aio & aio) const;
	int write(Sys::Aio & aio) const;
#endif
	int close();

	using Phy::ioctl;
	using Phy::read;
	using Phy::write;


	port_t port() const{ return periph_port & 0xFF; }

protected:
	u16 periph_port;
#ifndef __MCU_ONLY__
#else
	int flags;
#endif

	int open(const char * name, int flags);


private:
	static u16 fd_map[LINK_OPEN_MAX];

	void update_fileno() const;
	int lookup_fileno() const;

};

};

#endif /* PHY_PERIPH_HPP_ */
