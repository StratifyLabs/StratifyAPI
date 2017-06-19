//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Dev.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

Dev::Dev() {
	// TODO Auto-generated constructor stub
	m_fd = -1;
#if defined __link
	_driver = 0;
#endif
}



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
