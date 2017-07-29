/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_USB_HPP_
#define SAPI_USB_HPP_

#include <sos/dev/usb.h>
#include "Periph.hpp"

namespace hal {

/*! \brief USB Pin Assignment
 * \details This class allows simple manipulation of the usb_pin_assignment_t.
 */
class UsbPinAssignment : public PinAssignment<usb_pin_assignment_t>{};

/*! \brief USB Class
 * \details This class implements a USB transceiver.
 */
class Usb : public Periph<usb_info_t, usb_attr_t, 'u'> {
public:
	Usb(port_t port);

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

#endif /* SAPI_USB_HPP_ */
