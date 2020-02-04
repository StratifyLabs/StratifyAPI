/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include "hal/Usb.hpp"
using namespace hal;

Usb::Usb(port_t port) : Periph(CORE_PERIPH_USB, port){}


int Usb::reset() const {
	usb_attr_t attr;
	attr.o_flags = Usb::RESET;
	return set_attributes(attr);
}

int Usb::attach() const {
	usb_attr_t attr;
	attr.o_flags = Usb::ATTACH;
	return set_attributes(attr);
}

int Usb::configure() const {
	usb_attr_t attr;
	attr.o_flags = Usb::CONFIGURE;
	return set_attributes(attr);
}

int Usb::detach() const {
	usb_attr_t attr;
	attr.o_flags = Usb::DETACH;
	return set_attributes(attr);
}

int Usb::disable_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::DISABLE_ENDPOINT;
	attr.address = ep;
	return set_attributes(attr);
}

int Usb::enable_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::ENABLE_ENDPOINT;
	attr.address = ep;
	return set_attributes(attr);
}

bool Usb::is_connected() const {
	return ioctl(IoRequest(I_USB_ISCONNECTED));
}

int Usb::reset_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::RESET_ENDPOINT;
	attr.address = ep;
	return set_attributes(attr);
}

int Usb::set_addr(int addr) const {
	usb_attr_t attr;
	attr.o_flags = Usb::SET_ADDRESS;
	attr.address = addr;
	return set_attributes(attr);
}

int Usb::stall_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::STALL_ENDPOINT;
	attr.address = ep;
	return set_attributes(attr);
}

int Usb::unstall_endpoint(int ep) const {
	usb_attr_t attr;
	attr.o_flags = Usb::UNSTALL_ENDPOINT;
	attr.address = ep;
	return set_attributes(attr);
}
