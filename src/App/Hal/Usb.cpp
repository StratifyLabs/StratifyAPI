//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include "Hal/Usb.hpp"
using namespace Hal;

Usb::Usb(port_t port) : Periph(CORE_PERIPH_USB, port){}

int Usb::get_attr(usb_attr_t * attr){
	return ioctl(I_USB_GETATTR, attr);
}

int Usb::set_attr(usb_attr_t * attr){
	return ioctl(I_USB_GETATTR, attr);
}
int Usb::reset(){
	return ioctl(I_USB_RESET);
}
int Usb::attach(){
	return ioctl(I_USB_ATTACH);
}
int Usb::configure(){
	return ioctl(I_USB_CONFIGURE);
}

int Usb::detach(){
	return ioctl(I_USB_DETACH);
}

int Usb::disable_endpoint(int ep){
	return ioctl(I_USB_DISABLEEP, ep);
}

int Usb::enable_endpoint(int ep){
	return ioctl(I_USB_ENABLEEP, ep);
}

bool Usb::is_connected(){
	return ioctl(I_USB_ISCONNECTED);
}

int Usb::reset_endpoint(int ep){
	return ioctl(I_USB_RESETEP, ep);
}

int Usb::set_addr(int addr){
	return ioctl(I_USB_SETATTR, addr);
}

int Usb::stall_endpoint(int ep){
	return ioctl(I_USB_STALLEP, ep);
}

int Usb::unstall_endpoint(int ep){
	return ioctl(I_USB_UNSTALLEP, ep);
}
