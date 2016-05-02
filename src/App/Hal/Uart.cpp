//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include "Hal/Uart.hpp"
using namespace Hal;

Uart::Uart(port_t port) : Periph(CORE_PERIPH_UART, port) {}

int Uart::attr(uart_attr_t * attr){
	return ioctl(I_UART_GETATTR, attr);
}

int Uart::setattr(const uart_attr_t * attr){
	return ioctl(I_UART_SETATTR, attr);
}

int Uart::getbyte(char * c){
	return ioctl(I_UART_GETBYTE, c);
}

int Uart::flush(void){
	return ioctl(I_UART_FLUSH);
}

