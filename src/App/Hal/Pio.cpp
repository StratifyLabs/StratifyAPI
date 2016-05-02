//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include "Hal/Pio.hpp"
using namespace Hal;

Pio::Pio(port_t port) : Periph(CORE_PERIPH_PIO, port){}


int Pio::attr(pio_attr_t * attr){
	return ioctl(I_PIO_GETATTR, attr);
}

int Pio::setattr(const pio_attr_t * attr){
	return ioctl(I_PIO_SETATTR, attr);
}

int Pio::setaction(const pio_action_t * action){
	return ioctl(I_PIO_SETACTION, action);
}

int Pio::setmask(unsigned int mask){
	return ioctl(I_PIO_SETMASK, mask);
}

int Pio::clrmask(unsigned int mask){
	return ioctl(I_PIO_CLRMASK, mask);
}

unsigned int Pio::get(void){
	return ioctl(I_PIO_GET);
}

int Pio::set(unsigned int value){
	return ioctl(I_PIO_SET, value);
}

