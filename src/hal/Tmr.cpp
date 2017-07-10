//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Tmr.hpp"

using namespace hal;


Tmr::Tmr(port_t port) : Periph(CORE_PERIPH_TMR, port) {}


int Tmr::enable() const {
	return ioctl(I_TMR_ENABLE);
}

int Tmr::disable() const {
	return ioctl(I_TMR_DISABLE);
}

u32 Tmr::get_value() const {
	return ioctl(I_TMR_GET);
}

int Tmr::set_value(u32 value) const {
	return ioctl(I_TMR_SET, value);
}


