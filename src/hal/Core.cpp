//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include "hal/Core.hpp"
using namespace hal;

Core::Core(port_t port) : Periph(CORE_PERIPH_CORE, port){}


int Core::set_pin_function(const core_pinfunc_t & req){
	return ioctl(
				arg::IoRequest(I_CORE_SETPINFUNC),
				arg::IoConstArgument(&req)
				);
}

int Core::get_mcu_board_config(mcu_board_config_t & config){
	return ioctl(
				arg::IoRequest(I_CORE_GETMCUBOARDCONFIG),
				arg::IoArgument(&config)
				);
}

int Core::set_clock_divide(u32 value){
	return ioctl(
				arg::IoRequest(I_CORE_SETCLKDIVIDE),
				arg::IoIntArgument(value)
				);
}

void Core::reset(){
	core_attr_t attr;
	attr.o_flags = CORE_FLAG_EXEC_RESET;
	set_attributes(attr);
}

void Core::invoke_bootloader(){
	core_attr_t attr;
	attr.o_flags = CORE_FLAG_EXEC_INVOKE_BOOTLOADER;
	set_attributes(attr);
}

