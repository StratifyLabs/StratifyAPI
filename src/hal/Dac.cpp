//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include "hal/Dac.hpp"
#include <stdlib.h>

using namespace hal;

Dac::Dac(port_t port) : Periph(CORE_PERIPH_DAC, port){}

int Dac::get_attr(dac_attr_t * attr){
	return ioctl(I_DAC_GETATTR, attr);
}

int Dac::set_attr(const dac_attr_t * attr){
	return ioctl(I_DAC_SETATTR, attr);
}

dac_sample_t Dac::value(uint32_t channel){
	dac_reqattr_t attr;
	attr.channel = channel;
	ioctl(I_DAC_VALUE, &attr);
	return attr.value;
}

int Dac::set(dac_sample_t value, uint32_t channel){
	dac_reqattr_t attr;
	attr.channel = channel;
	attr.value = value;
	return ioctl(I_DAC_SET, &attr);
}


