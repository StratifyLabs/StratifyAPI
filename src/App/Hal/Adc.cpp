//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "Hal/Adc.hpp"
using namespace Hal;

Adc::Adc(port_t port) : Periph(CORE_PERIPH_ADC, port){}

int Adc::attr(adc_attr_t * attr){
	return ioctl(I_ADC_GETATTR, attr);
}

int Adc::setattr(const adc_attr_t * attr){
	return ioctl(I_ADC_SETATTR, (void*)attr);
}

