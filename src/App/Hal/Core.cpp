//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include "Hal/Core.hpp"
using namespace Hal;

Core::Core(port_t port) : Periph(CORE_PERIPH_CORE, port){}

int Core::attr(core_attr_t * attr){
	return ioctl(I_CORE_GETATTR, attr);
}

int Core::setattr(const core_attr_t * attr){
	return ioctl(I_CORE_SETATTR, attr);
}

int Core::setpinfunc(const core_pinfunc_t * req){
	return ioctl(I_CORE_SETPINFUNC, req);
}

int Core::sleep(core_sleep_t level){
	return ioctl(I_CORE_SLEEP, level);
}

void Core::reset(void){
	ioctl(I_CORE_RESET);
}

void Core::invokebootloader(void){
	ioctl(I_CORE_INVOKEBOOTLOADER);
}

int Core::setirqprio(const core_irqprio_t & req){
	return -1;
}

int Core::setclkout(const core_clkout_t * clkout){
	return ioctl(I_CORE_SETCLKOUT, clkout);
}
