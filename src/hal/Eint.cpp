//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved




#include <unistd.h>
#include <fcntl.h>
#include "hal/Eint.hpp"
using namespace hal;

Eint::Eint(port_t port) : Periph(CORE_PERIPH_EINT, port){}
