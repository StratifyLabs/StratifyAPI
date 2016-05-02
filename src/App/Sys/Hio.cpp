//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "Sys/Hio.hpp"
using namespace Sys;

int Hio::attr(hio_attr_t & attr){
	//return ioctl(I_HIO_GETATTR, &attr);
	return 0;
}

int Hio::report(void * dest, int size){
	return ioctl(I_HIO_REPORT, dest);
}

int Hio::setreport(const void * src, int size){
	return ioctl(I_HIO_SETREPORT, src);
}
