/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_USB_HPP_
#define STFY_APP_USB_HPP_

#include <iface/dev/usb.h>
#include "Periph.hpp"

namespace Hal {

/*! \brief USB Class
 * \details This class implements a USB transceiver.
 */
class Usb : public Periph {
public:
	Usb(port_t port);

	/*! \brief Get USB attributes */
	int attr(usb_attr_t * attr);
	/*! \brief Set USB attributes */
	int setattr(usb_attr_t * attr);
	/*! \brief Reset USB */
	int reset(void);
	int attach(void);
	int configure(void);
	int detach(void);
	int disableep(int ep);
	int enableep(int ep);
	bool isconnected(void);
	int resetep(int ep);
	int setaddr(int addr);
	int stallep(int ep);
	int unstallep(int ep);

#ifdef __MCU_ONLY__
	using Pblock::read;
	using Pblock::write;
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int read(Aio & aio);
	int write(Aio & aio);
	int close(void);
#endif

private:

};

};

#endif /* STFY_APP_USB_HPP_ */
