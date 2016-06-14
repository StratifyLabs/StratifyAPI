//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "hal/Adc.hpp"

using namespace hal;

Adc::Adc(port_t port) : Periph(CORE_PERIPH_ADC, port){}

int Adc::get_attr(adc_attr_t * attr){
	return ioctl(I_ADC_GETATTR, attr);
}

int Adc::set_attr(const adc_attr_t * attr){
	return ioctl(I_ADC_SETATTR, (void*)attr);
}

