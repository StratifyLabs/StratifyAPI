//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Device.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

#if defined __link
Device::Device(link_transport_mdriver_t * d) : File(d){}
#endif

Device::Device(){}


#ifndef __link
int Device::read(sys::Aio & aio) const {
	aio.m_aio_var.aio_fildes = m_fd;
	return ::aio_read(&(aio.m_aio_var));
}

int Device::write(sys::Aio & aio) const {
	aio.m_aio_var.aio_fildes = m_fd;
	return ::aio_write(&(aio.m_aio_var));
}
#endif
