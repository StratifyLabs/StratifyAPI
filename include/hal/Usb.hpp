/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_HAL_USB_HPP_
#define SAPI_HAL_USB_HPP_

#include <sos/dev/usb.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"

namespace hal {

class UsbFlags {
public:
	enum flags {
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
};


/*! \brief USB Pin Assignment
 * \details This class allows simple manipulation of the usb_pin_assignment_t.
 */
class UsbPinAssignment : public PinAssignment<usb_pin_assignment_t>{};

class UsbAttributes :
		public PinAssignmentPeriphAttributes<usb_attr_t, usb_pin_assignment_t>,
		public UsbFlags {

};

/*! \brief USB Class
 * \details This class implements a USB transceiver.
 */
class Usb :
		public Periph<usb_info_t, usb_attr_t, UsbAttributes, 'u'>,
		public UsbFlags {
public:
	Usb(port_t port);

	int reset() const;
	int attach() const;
	int configure() const;
	int detach() const;
	int disable_endpoint(int ep) const;
	int enable_endpoint(int ep) const;
	bool is_connected() const;
	int reset_endpoint(int ep) const;
	int set_addr(int addr) const;
	int stall_endpoint(int ep) const;
	int unstall_endpoint(int ep) const;

private:

};

}

#endif /* SAPI_HAL_USB_HPP_ */
