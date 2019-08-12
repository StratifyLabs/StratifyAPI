/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "hal/CFifo.hpp"
#include "hal/Fifo.hpp"

namespace hal {

int CFifo::get_count() const {
	cfifo_info_t info;
	if( get_info(info) < 0 ){ return -1; }
	return info.count;
}

int CFifo::get_info(cfifo_info_t & info) const {
	return ioctl(
				arg::IoRequest(I_CFIFO_GETINFO),
				arg::IoArgument(&info)
				);
}

int CFifo::get_owner(int channel, int & owner) const {
	mcu_channel_t c;
	int ret;
	c.loc = channel;
	c.value = (u32)-1;
	ret = ioctl(
				arg::IoRequest(I_CFIFO_GETOWNER),
				arg::IoArgument(&c)
				);
	if( ret >= 0 ){
		owner = c.value;
	}
	return ret;
}

int CFifo::set_owner(int channel, int owner) const {
	mcu_channel_t c;
	c.loc = channel;
	c.value = owner;
	return ioctl(
				arg::IoRequest(I_CFIFO_SETOWNER),
				arg::IoArgument(&c)
				);
}

int CFifo::get_info(int channel, fifo_info_t & info) const {
	cfifo_fifoinfo_t fifo_info;
	fifo_info.channel = channel;
	if( ioctl(
			 arg::IoRequest(I_CFIFO_FIFOGETINFO),
			 arg::IoArgument(&fifo_info)
			 ) >= 0 ){
		info = fifo_info.info;
	}
	return 0;
}

FifoInfo CFifo::get_info(int channel){
	FifoInfo info;
	get_info(channel, info.m_info);
	return info;
}


int CFifo::set_attributes(int channel, const fifo_attr_t & attr) const {
	cfifo_fifoattr_t fifo_attr;
	fifo_attr.channel = channel;
	fifo_attr.attr = attr;
	return ioctl(
				arg::IoRequest(I_CFIFO_FIFOSETATTR),
				arg::IoArgument(&fifo_attr)
				);
}

int CFifo::initialize(int channel) const {
	cfifo_fiforequest_t request;
	request.channel = channel;
	return ioctl(
				arg::IoRequest(I_CFIFO_FIFOINIT),
				arg::IoArgument(&request)
				);
}

int CFifo::flush(int channel) const {
	cfifo_fiforequest_t request;
	request.channel = channel;
	return ioctl(
				arg::IoRequest(I_CFIFO_FIFOFLUSH),
				arg::IoArgument(&request)
				);
}

int CFifo::exit(int channel) const {
	cfifo_fiforequest_t request;
	request.channel = channel;
	return ioctl(
				arg::IoRequest(I_CFIFO_FIFOEXIT),
				arg::IoArgument(&request)
				);
}

int CFifo::set_writeblock(int channel, bool value) const {
	fifo_attr_t attr;
	attr.o_flags = Fifo::FLAG_SET_WRITEBLOCK;
	if( value == false ){
		attr.o_flags |= Fifo::FLAG_IS_OVERFLOW;
	}
	return set_attributes(channel, attr);
}


} /* namespace hal */
