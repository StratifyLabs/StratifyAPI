/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "hal/CFifo.hpp"
#include "hal/Fifo.hpp"

namespace hal {

#if defined __link
CFifo::CFifo(link_transport_mdriver_t * d) : Dev(d){}
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
	ret = ioctl_fifo(channel, I_CFIFO_GETOWNER, &c);
	if( ret >= 0 ){
		owner = c.value;
	}
	return ret;
}

int CFifo::set_owner(int channel, int owner) const {
	mcu_channel_t c;
	c.loc = channel;
	c.value = owner;
	return ioctl_fifo(channel, I_CFIFO_SETOWNER, &c);
}

int CFifo::get_info(int channel, fifo_info_t & info) const {
	return ioctl_fifo(channel, I_FIFO_GETINFO, &info);
}

int CFifo::set_attr(int channel, const fifo_attr_t & attr) const {
	return ioctl_fifo(channel, I_FIFO_SETATTR, (void*)&attr);
}

int CFifo::init(int channel) const {
	return ioctl_fifo(channel, I_FIFO_INIT);
}

int CFifo::flush(int channel) const {
	return ioctl_fifo(channel, I_FIFO_FLUSH);
}

int CFifo::exit(int channel) const {
	return ioctl_fifo(channel, I_FIFO_EXIT);
}

int CFifo::set_writeblock(int channel, bool value) const {
	fifo_attr_t attr;
	attr.o_flags = Fifo::FLAG_SET_WRITEBLOCK;
	if( value == false ){
		attr.o_flags |= Fifo::FLAG_IS_OVERFLOW;
	}
	return ioctl_fifo(channel, I_FIFO_SETATTR, &attr);
}


int CFifo::ioctl_fifo(int channel, int request, void * ctl) const {
	cfifo_fiforequest_t fifo_request;
	fifo_request.channel = channel;
	fifo_request.request = request;
	fifo_request.ctl = ctl;
	return ioctl(I_CFIFO_FIFOREQUEST, &fifo_request);
}

} /* namespace hal */
