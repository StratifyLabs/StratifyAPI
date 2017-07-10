//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include "hal/Dac.hpp"
#include <stdlib.h>

using namespace hal;

Dac::Dac(port_t port) : Periph(CORE_PERIPH_DAC, port){}


