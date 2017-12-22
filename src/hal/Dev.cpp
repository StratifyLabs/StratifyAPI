//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Dev.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

#if defined __link
Dev::Dev(link_transport_mdriver_t * d) : File(d){}
#endif

Dev::Dev(){}




#ifndef __link
int Dev::read(sys::Aio & aio) const {
	aio.m_aio_var.aio_fildes = m_fd;
	return ::aio_read(&(aio.m_aio_var));
}

int Dev::write(sys::Aio & aio) const {
	aio.m_aio_var.aio_fildes = m_fd;
	return ::aio_write(&(aio.m_aio_var));
}
#endif
