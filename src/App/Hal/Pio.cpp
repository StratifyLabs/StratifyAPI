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

int Pio::set_attr(const pio_attr_t * attr){
	return ioctl(I_PIO_SETATTR, attr);
}

int Pio::set_action(const pio_action_t * action){
	return ioctl(I_PIO_SETACTION, action);
}

int Pio::set_mask(unsigned int mask){
	return ioctl(I_PIO_SETMASK, mask);
}

int Pio::clear_mask(unsigned int mask){
	return ioctl(I_PIO_CLRMASK, mask);
}

u32 Pio::value(void){
	return ioctl(I_PIO_GET);
}

int Pio::set(unsigned int value){
	return ioctl(I_PIO_SET, value);
}

