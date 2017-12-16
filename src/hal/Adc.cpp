//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "hal/Adc.hpp"

using namespace hal;

Adc::Adc(port_t port) : Periph(CORE_PERIPH_ADC, port){}


