//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Fifo.hpp"
using namespace hal;

Fifo::Fifo(){}

int Fifo::get_info(fifo_info_t & info) const { return ioctl(I_FIFO_GETINFO, (void*)&info); }
int Fifo::flush() const { return ioctl(I_FIFO_FLUSH); }
int Fifo::init() const { return ioctl(I_FIFO_INIT); }
int Fifo::exit() const { return ioctl(I_FIFO_EXIT); }
int Fifo::set_writeblock() const {
	fifo_attr_t attr;
	attr.o_flags = FIFO_FLAG_SET_WRITEBLOCK;
	return ioctl(I_FIFO_SETATTR, &attr);
}
