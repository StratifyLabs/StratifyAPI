//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "hal/Spi.hpp"

using namespace hal;

Spi::Spi(port_t port) : Periph(CORE_PERIPH_SPI, port){}


int Spi::get_attr(spi_attr_t * attr){
	return ioctl(I_SPI_GETATTR, attr);
}

int Spi::set_attr(const spi_attr_t * attr){
	return ioctl(I_SPI_SETATTR, attr);
}

int Spi::swap(int byte){
	return ioctl(I_SPI_SWAP, byte);
}

int Spi::set_duplex(void * buf){
	return ioctl(I_SPI_SETDUPLEX, buf);
}

