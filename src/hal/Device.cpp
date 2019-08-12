//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal/Device.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

Device::Device(){
	set_keep_open(true);
}

#ifndef __link
int Device::read(fs::Aio & aio) const {
	aio.m_aio_var.aio_fildes = fileno();
	return ::aio_read(&(aio.m_aio_var));
}

int Device::write(fs::Aio & aio) const {
	aio.m_aio_var.aio_fildes = fileno();
	return ::aio_write(&(aio.m_aio_var));
}

int Device::cancel_read(int channel){
	return cancel(channel, MCU_EVENT_FLAG_DATA_READY);
}

int Device::cancel_write(int channel){
	return cancel(channel, MCU_EVENT_FLAG_WRITE_COMPLETE);
}

int Device::cancel(int channel, int o_events){
	mcu_action_t action;
	memset(&action, 0, sizeof(action));
	action.channel = channel;
	action.o_events = o_events;
	return ioctl(
				arg::IoRequest(I_MCU_SETACTION),
				arg::IoArgument(&action)
				);
}

#endif


int Device::set_interrupt_priority(int priority, int request){
	mcu_action_t action;
	memset(&action, 0, sizeof(action));
	action.o_events = MCU_EVENT_FLAG_SET_PRIORITY;
	action.prio = priority;
	return ioctl(
				arg::IoRequest(request),
				arg::IoArgument(&action)
				);
}




