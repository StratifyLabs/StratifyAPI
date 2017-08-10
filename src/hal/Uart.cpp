//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Uart.hpp"
using namespace hal;

Uart::Uart(port_t port) : Periph(CORE_PERIPH_UART, port) {}

int Uart::get(char & c){
	return ioctl(I_UART_GET, &c);
}

int Uart::put(char c){
	return ioctl(I_UART_PUT, c);
}

int Uart::flusheight(){
	return ioctl(I_UART_FLUSH);
}

