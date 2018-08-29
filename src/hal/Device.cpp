//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal/Device.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

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


int Device::set_interrupt_priority(int priority, int request){
    mcu_action_t action;
    memset(&action, 0, sizeof(action));
    action.o_events = MCU_EVENT_FLAG_SET_PRIORITY;
    action.prio = priority;
    return ioctl(request, &action);
}
