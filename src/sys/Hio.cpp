//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "sys/Hio.hpp"
using namespace sys;

int Hio::attr(hio_attr_t & attr){
	//return ioctl(I_HIO_GETATTR, &attr);
	return 0;
}

int Hio::report(void * dest, int size){
	return ioctl(I_HIO_REPORT, dest);
}

int Hio::set_report(const void * src, int size){
	return ioctl(I_HIO_SETREPORT, src);
}
