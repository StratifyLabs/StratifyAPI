//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "hal/Adc.hpp"

using namespace hal;

Adc::Adc(port_t port) : Periph(CORE_PERIPH_ADC, port){}

AdcInfo Adc::get_info() const {
	AdcInfo result;
	ioctl(I_ADC_GETINFO, &result.info());
	return result;
}



