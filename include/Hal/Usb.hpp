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

	int get_attr(usb_attr_t * attr);
	int set_attr(usb_attr_t * attr);
	int reset();
	int attach();
	int configure();
	int detach();
	int disable_endpoint(int ep);
	int enable_endpoint(int ep);
	bool is_connected();
	int reset_endpoint(int ep);
	int set_addr(int addr);
	int stall_endpoint(int ep);
	int unstall_endpoint(int ep);

#ifdef __MCU_ONLY__
	using Pblock::read;
	using Pblock::write;
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int read(Aio & aio);
	int write(Aio & aio);
	int close();
#endif

private:

};

};

#endif /* STFY_APP_USB_HPP_ */
