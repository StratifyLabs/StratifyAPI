//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Uart.hpp"
using namespace hal;

Uart::Uart(port_t port) : Periph(CORE_PERIPH_UART, port) {}

int Uart::get_attr(uart_attr_t & attr){
	return ioctl(I_UART_GETATTR, &attr);
}

int Uart::set_attr(const uart_attr_t & attr){
	return ioctl(I_UART_SETATTR, &attr);
}

int Uart::get_byte(char & c){
	return ioctl(I_UART_GETBYTE, &c);
}

int Uart::flush(){
	return ioctl(I_UART_FLUSH);
}

