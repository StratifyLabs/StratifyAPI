/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal/Led.hpp"

namespace hal {

Led::Led(){}


int Led::enable(u32 duty_cycle, u32 period) const{
	led_attr_t attr;
	attr.o_flags = FLAG_ENABLE;
	if( period ){
		attr.duty_cycle = duty_cycle;
		attr.period = period;
		attr.o_flags |= FLAG_IS_DUTY_CYCLE;
	}
	return ioctl(
				arg::IoRequest(I_LED_SETATTR),
				arg::IoArgument(&attr)
				);
}

int Led::disable(bool high_impedance) const {
	led_attr_t attr;
	attr.o_flags = FLAG_DISABLE;
	if( high_impedance ){
		attr.o_flags |= FLAG_IS_HIGH_IMPEDANCE;
	}
	return ioctl(
				arg::IoRequest(I_LED_SETATTR),
				arg::IoArgument(&attr)
				);
}

int Led::get_info(led_info_t & info) const {
	return ioctl(
				arg::IoRequest(I_LED_GETINFO),
				arg::IoArgument(&info)
				);
}

} /* namespace hal */
