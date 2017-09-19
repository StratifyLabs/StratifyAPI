/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "hal/Mcfifo.hpp"

namespace hal {

Mcfifo::Mcfifo(){}


int Mcfifo::get_count() const {
	mcfifo_info_t info;
	if( get_info(info) < 0 ){ return -1; }
	return info.count;
}

int Mcfifo::get_info(mcfifo_info_t & info) const {
	return ioctl(I_MCFIFO_GETINFO, &info);
}

int Mcfifo::get_owner(int channel) const {
	mcu_channel_t c;
	c.loc = channel;
	c.value = (u32)-1;
	return ioctl_fifo(channel, I_MCFIFO_GETOWNER, &c);
}

int Mcfifo::set_owner(int channel, int owner) const {
	mcu_channel_t c;
	c.loc = channel;
	c.value = owner;
	return ioctl_fifo(channel, I_MCFIFO_SETOWNER, &c);
}

int Mcfifo::get_fifo_info(int channel, fifo_info_t & info) const {
	return ioctl_fifo(channel, I_FIFO_GETINFO, &info);
}

int Mcfifo::set_fifo_attr(int channel, fifo_attr_t & attr) const {
	return ioctl_fifo(channel, I_FIFO_SETATTR, &attr);
}

int Mcfifo::init_fifo(int channel) const {
	return ioctl_fifo(channel, I_FIFO_INIT);
}

int Mcfifo::flush_fifo(int channel) const {
	return ioctl_fifo(channel, I_FIFO_FLUSH);
}

int Mcfifo::exit_fifo(int channel) const {
	return ioctl_fifo(channel, I_FIFO_EXIT);
}


int Mcfifo::ioctl_fifo(int channel, int request, void * ctl) const {
	mcfifo_fiforequest_t fifo_request;
	fifo_request.channel = channel;
	fifo_request.request = request;
	fifo_request.ctl = ctl;
	return ioctl(I_MCFIFO_FIFOREQUEST, &fifo_request);
}

} /* namespace hal */
