//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Sys/Fifo.hpp"
using namespace Sys;

Fifo::Fifo(){}

int Fifo::attr(fifo_attr_t * attr){ return ioctl(I_FIFO_GETATTR, (void*)attr); }
int Fifo::flush(){ return ioctl(I_FIFO_FLUSH); }
int Fifo::init(){ return ioctl(I_FIFO_INIT); }
int Fifo::exit(){ return ioctl(I_FIFO_EXIT); }
int Fifo::setwriteblock(){ return ioctl(I_FIFO_SETWRITEBLOCK); }
