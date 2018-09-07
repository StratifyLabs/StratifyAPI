#ifndef SAPI_HAL_STREAMFFIFO_HPP
#define SAPI_HAL_STREAMFFIFO_HPP

#include <sos/dev/stream_ffifo.h>

#include "../api/HalObject.hpp"
#include "FFifo.hpp"

namespace hal {

class StreamFFifoChannelInfo : public api::HalInfoObject {
public:

	StreamFFifoChannelInfo(const stream_ffifo_channel_info_t & info) : m_info(info){}
	bool is_valid() const {
		return m_info.ffifo.frame_count != 0;
	}

	FFifoInfo ffifo_info() const { return m_info.ffifo; }

private:
	const stream_ffifo_channel_info_t & m_info;
};

class StreamFFifoInfo : public api::HalInfoObject {
public:
	StreamFFifoInfo(): m_receive(m_info.rx), m_transmit(m_info.tx){
		memset(&m_info, 0, sizeof(m_info));
	}

	operator stream_ffifo_info_t & (){ return m_info; }
	operator const stream_ffifo_info_t & () const { return m_info; }


	bool is_valid() const {
		return receive().is_valid() || transmit().is_valid();
	}

	const StreamFFifoChannelInfo & receive() const { return m_receive; }
	const StreamFFifoChannelInfo & transmit() const { return m_transmit; }

private:
	stream_ffifo_info_t m_info;
	StreamFFifoChannelInfo m_receive;
	StreamFFifoChannelInfo m_transmit;

};

class StreamFFifoAttributes : public api::HalInfoObject {
public:
	StreamFFifoAttributes(){
		memset(&m_attributes, 0, sizeof(m_attributes));
	}

	operator const stream_ffifo_attr_t & () const { return m_attributes; }

	void set_flags(u32 value){
		m_attributes.o_flags = value;
	}

private:
	stream_ffifo_attr_t m_attributes;

};

typedef StreamFFifoAttributes StreamFFifoAttr;


class StreamFFifo : public Device {
public:
	StreamFFifo();

	enum {
		START = STREAM_FFIFO_FLAG_START,
		STOP = STREAM_FFIFO_FLAG_STOP,
		FLUSH = STREAM_FFIFO_FLAG_FLUSH
	};

	int set_attributes(const stream_ffifo_attr_t & attributes){
		return ioctl(I_STREAM_FFIFO_SETATTR, &attributes);
	}

	int start();
	int stop();
	int flush();

	int get_info(stream_ffifo_info_t & info);


};

}

#endif // SAPI_HAL_STREAMFFIFO_HPP
