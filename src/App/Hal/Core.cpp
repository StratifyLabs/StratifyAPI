//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include <hal/Core.hpp>
using namespace hal;

Core::Core(port_t port) : Periph(CORE_PERIPH_CORE, port){}

int Core::get_attr(core_attr_t * attr){
	return ioctl(I_CORE_GETATTR, attr);
}

int Core::set_attr(const core_attr_t * attr){
	return ioctl(I_CORE_SETATTR, attr);
}

int Core::set_pin_function(const core_pinfunc_t * req){
	return ioctl(I_CORE_SETPINFUNC, req);
}

int Core::sleep(core_sleep_t level){
	return ioctl(I_CORE_SLEEP, level);
}

void Core::reset(){
	ioctl(I_CORE_RESET);
}

void Core::invoke_bootloader(){
	ioctl(I_CORE_INVOKEBOOTLOADER);
}

int Core::set_clkout(const core_clkout_t * clkout){
	return ioctl(I_CORE_SETCLKOUT, clkout);
}
