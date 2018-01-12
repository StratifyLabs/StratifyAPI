/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "hal/CFifo.hpp"
#include "hal/Fifo.hpp"

namespace hal {

#if defined __link
CFifo::CFifo(link_transport_mdriver_t * d) : Device(d){}
#endif

CFifo::CFifo(){}

int CFifo::get_count() const {
	cfifo_info_t info;
	if( get_info(info) < 0 ){ return -1; }
	return info.count;
}

int CFifo::get_info(cfifo_info_t & info) const {
	return ioctl(I_CFIFO_GETINFO, &info);
}

int CFifo::get_owner(int channel, int & owner) const {
	mcu_channel_t c;
	int ret;
	c.loc = channel;
	c.value = (u32)-1;
	ret = ioctl(I_CFIFO_GETOWNER, &c);
	if( ret >= 0 ){
		owner = c.value;
	}
	return ret;
}

int CFifo::set_owner(int channel, int owner) const {
	mcu_channel_t c;
	c.loc = channel;
	c.value = owner;
	return ioctl(I_CFIFO_SETOWNER, &c);
}

int CFifo::get_info(int channel, fifo_info_t & info) const {
	cfifo_fifoinfo_t fifo_info;
	fifo_info.channel = channel;
	if( ioctl(I_CFIFO_FIFOGETINFO, &fifo_info) >= 0 ){
		info = fifo_info.info;
	}
	return 0;
}

int CFifo::set_attr(int channel, const fifo_attr_t & attr) const {
	cfifo_fifoattr_t fifo_attr;
	fifo_attr.channel = channel;
	fifo_attr.attr = attr;
	return ioctl(I_CFIFO_FIFOSETATTR, &fifo_attr);
}

int CFifo::init(int channel) const {
	cfifo_fiforequest_t request;
	request.channel = channel;
	return ioctl(I_CFIFO_FIFOINIT, &request);}

int CFifo::flush(int channel) const {
	cfifo_fiforequest_t request;
	request.channel = channel;
	return ioctl(I_CFIFO_FIFOFLUSH, &request);
}

int CFifo::exit(int channel) const {
	cfifo_fiforequest_t request;
	request.channel = channel;
	return ioctl(I_CFIFO_FIFOEXIT, &request);}

int CFifo::set_writeblock(int channel, bool value) const {
	fifo_attr_t attr;
	attr.o_flags = Fifo::FLAG_SET_WRITEBLOCK;
	if( value == false ){
		attr.o_flags |= Fifo::FLAG_IS_OVERFLOW;
	}
	return set_attr(channel, attr);
}


} /* namespace hal */
