//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal/Usb.hpp"
using namespace hal;

Usb::Usb(port_t port) : Periph(CORE_PERIPH_USB, port){}


int Usb::reset() const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_RESET;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::attach() const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_ATTACH;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::configure() const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_CONFIGURE;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::detach() const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_DETACH;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::disable_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_DISABLE_ENDPOINT;
	attr.address = ep;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::enable_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_ENABLE_ENDPOINT;
	attr.address = ep;
	return ioctl(I_USB_SETATTR, &attr);
}

bool Usb::is_connected() const {
	return ioctl(I_USB_ISCONNECTED);
}

int Usb::reset_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_RESET_ENDPOINT;
	attr.address = ep;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::set_addr(int addr) const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_SET_ADDRESS;
	attr.address = addr;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::stall_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_STALL_ENDPOINT;
	attr.address = ep;
	return ioctl(I_USB_SETATTR, &attr);
}

int Usb::unstall_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::FLAG_UNSTALL_ENDPOINT;
	attr.address = ep;
	return ioctl(I_USB_SETATTR, &attr);
}
