#include "hal/FFifo.hpp"

using namespace hal;

FFifo::FFifo(){}

int FFifo::initialize(const arg::SourceFilePath & path, const FFifoAttributes & attributes, const fs::OpenFlags & flags){
	int ret;
	ret = open(
				arg::FilePath(path.argument()),
				fs::OpenFlags(flags).set_read_write()
				);
	if( ret < 0 ){
		return ret;
	}

	ret = set_attributes(attributes);
	if( ret < 0 ){
		return ret;
	}

	return set_error_number_if_error(
				ioctl(arg::IoRequest(I_FFIFO_INIT))
				);
}

int FFifo::flush() const {
	return set_error_number_if_error(
				ioctl(arg::IoRequest(I_FFIFO_FLUSH))
				);
}

int FFifo::set_attributes(const FFifoAttributes & attr) const {
	return set_error_number_if_error(
				ioctl(
					arg::IoRequest(I_FFIFO_SETATTR),
					arg::IoConstArgument(&attr.m_ffifo_attr)
					)
				);
}

FFifoInfo FFifo::get_info() const {
	FFifoInfo info;
	set_error_number_if_error(
				ioctl(
					arg::IoRequest(I_FFIFO_GETINFO),
					arg::IoArgument(&info.m_ffifo_info)
					)
				);
	return info;
}

int FFifo::get_info(FFifoInfo & info) const {
	return set_error_number_if_error(
				ioctl(
					arg::IoRequest(I_FFIFO_GETINFO),
					arg::IoArgument(&info.m_ffifo_info)
					)
				);
}
