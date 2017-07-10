//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "hal/Spi.hpp"

using namespace hal;

Spi::Spi(port_t port) : Periph(CORE_PERIPH_SPI, port){}


int Spi::swap(int byte){
	return ioctl(I_SPI_SWAP, byte);
}

