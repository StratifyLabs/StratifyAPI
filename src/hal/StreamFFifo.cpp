#include "hal/StreamFFifo.hpp"

using namespace hal;

StreamFFifo::StreamFFifo()
{

}

int StreamFFifo::get_info(stream_ffifo_info_t & info){
	return ioctl(
				IoRequest(I_STREAM_FFIFO_GETINFO),
				IoArgument(&info)
				);
}

StreamFFifoInfo StreamFFifo::get_info(){
	stream_ffifo_info_t info;
	if( ioctl(
				IoRequest(I_STREAM_FFIFO_GETINFO),
				IoArgument(&info)
				) < 0 ){
		return StreamFFifoInfo();
	}

	return StreamFFifoInfo(info);
}



int StreamFFifo::start(){
	StreamFFifoAttributes attributes;
	attributes.set_flags(START);
	return set_attributes(attributes);
}

int StreamFFifo::stop(){
	StreamFFifoAttributes attributes;
	attributes.set_flags(STOP);
	return set_attributes(attributes);
}

int StreamFFifo::flush(){
	StreamFFifoAttributes attributes;
	attributes.set_flags(FLUSH);
	return set_attributes(attributes);
}
