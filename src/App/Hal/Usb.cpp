//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include "Hal/Usb.hpp"
using namespace Hal;

Usb::Usb(port_t port) : Periph(CORE_PERIPH_USB, port){}

int Usb::attr(usb_attr_t * attr){
	return ioctl(I_USB_GETATTR, attr);
}

int Usb::setattr(usb_attr_t * attr){
	return ioctl(I_USB_GETATTR, attr);
}
int Usb::reset(void){
	return ioctl(I_USB_RESET);
}
int Usb::attach(void){
	return ioctl(I_USB_ATTACH);
}
int Usb::configure(void){
	return ioctl(I_USB_CONFIGURE);
}

int Usb::detach(void){
	return ioctl(I_USB_DETACH);
}

int Usb::disableep(int ep){
	return ioctl(I_USB_DISABLEEP, ep);
}

int Usb::enableep(int ep){
	return ioctl(I_USB_ENABLEEP, ep);
}

bool Usb::isconnected(void){
	return ioctl(I_USB_ISCONNECTED);
}

int Usb::resetep(int ep){
	return ioctl(I_USB_RESETEP, ep);
}

int Usb::setaddr(int addr){
	return ioctl(I_USB_SETATTR, addr);
}

int Usb::stallep(int ep){
	return ioctl(I_USB_STALLEP, ep);
}

int Usb::unstallep(int ep){
	return ioctl(I_USB_UNSTALLEP, ep);
}
