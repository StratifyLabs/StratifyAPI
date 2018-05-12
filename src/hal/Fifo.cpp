//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Fifo.hpp"
using namespace hal;

Fifo::Fifo(){}

int Fifo::get_info(FifoInfo & info) const { return ioctl(I_FIFO_GETINFO, (void*)&info.m_info); }
const FifoInfo Fifo::get_info() const {
    FifoInfo a;
    set_error_number_if_error(get_info(a));
    return a;
}
int Fifo::flush() const { return ioctl(I_FIFO_FLUSH); }
int Fifo::init() const { return ioctl(I_FIFO_INIT); }
int Fifo::exit() const { return ioctl(I_FIFO_EXIT); }
int Fifo::set_writeblock(bool value) const {
	fifo_attr_t attr;
	attr.o_flags = FLAG_SET_WRITEBLOCK;
	if( value == false ){
		attr.o_flags |= FLAG_IS_OVERFLOW;
	}
	return ioctl(I_FIFO_SETATTR, &attr);
}
