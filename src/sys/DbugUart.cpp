//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys/DbugUart.hpp"
using namespace hal;
using namespace sys;

DbugUart::DbugUart(port_t port) :
	Uart(port)
{
	// TODO Auto-generated constructor stub

}

int DbugUart::open(int mode){
	return Uart::open(mode);
}

int DbugUart::close(void){
	return Uart::close();
}

int DbugUart::read(void * buf, int nbyte){
	return Uart::read(buf, nbyte);
}

int DbugUart::write(const void * buf, int nbyte){
	return Uart::write(buf, nbyte);
}
