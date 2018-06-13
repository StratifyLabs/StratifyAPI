/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_USB_HPP_
#define SAPI_USB_HPP_

#include <sos/dev/usb.h>
#include "PinAssignment.hpp"
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

	enum {
		FLAG_SET_UNCONFIGURED= USB_FLAG_SET_UNCONFIGURED,
		FLAG_SET_DEVICE = USB_FLAG_SET_DEVICE,
		FLAG_SET_HOST = USB_FLAG_SET_HOST,
		FLAG_SET_OTG = USB_FLAG_SET_OTG,
		FLAG_RESET = USB_FLAG_RESET,
		FLAG_ATTACH = USB_FLAG_ATTACH,
		FLAG_DETACH = USB_FLAG_DETACH,
		FLAG_CONFIGURE = USB_FLAG_CONFIGURE,
		FLAG_UNCONFIGURE = USB_FLAG_UNCONFIGURE,
		FLAG_SET_ADDRESS = USB_FLAG_SET_ADDRESS,
		FLAG_RESET_ENDPOINT = USB_FLAG_RESET_ENDPOINT,
		FLAG_ENABLE_ENDPOINT = USB_FLAG_ENABLE_ENDPOINT,
		FLAG_DISABLE_ENDPOINT = USB_FLAG_DISABLE_ENDPOINT,
		FLAG_STALL_ENDPOINT = USB_FLAG_STALL_ENDPOINT,
		FLAG_UNSTALL_ENDPOINT = USB_FLAG_UNSTALL_ENDPOINT,
        FLAG_CONFIGURE_ENDPOINT = USB_FLAG_CONFIGURE_ENDPOINT,

        SET_UNCONFIGURED= USB_FLAG_SET_UNCONFIGURED,
        SET_DEVICE = USB_FLAG_SET_DEVICE,
        SET_HOST = USB_FLAG_SET_HOST,
        SET_OTG = USB_FLAG_SET_OTG,
        RESET = USB_FLAG_RESET,
        ATTACH = USB_FLAG_ATTACH,
        DETACH = USB_FLAG_DETACH,
        CONFIGURE = USB_FLAG_CONFIGURE,
        UNCONFIGURE = USB_FLAG_UNCONFIGURE,
        SET_ADDRESS = USB_FLAG_SET_ADDRESS,
        RESET_ENDPOINT = USB_FLAG_RESET_ENDPOINT,
        ENABLE_ENDPOINT = USB_FLAG_ENABLE_ENDPOINT,
        DISABLE_ENDPOINT = USB_FLAG_DISABLE_ENDPOINT,
        STALL_ENDPOINT = USB_FLAG_STALL_ENDPOINT,
        UNSTALL_ENDPOINT = USB_FLAG_UNSTALL_ENDPOINT,
        CONFIGURE_ENDPOINT = USB_FLAG_CONFIGURE_ENDPOINT
	};

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

private:

};

}

#endif /* SAPI_USB_HPP_ */
