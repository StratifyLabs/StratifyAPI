//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal/Pio.hpp"
using namespace hal;

Pio::Pio(port_t port) : Periph(CORE_PERIPH_PIO, port){}



int Pio::set_mask(u32 mask) const {
	return ioctl(
				arg::IoRequest(I_PIO_SETMASK),
				arg::IoIntArgument(mask)
				);
}

int Pio::clear_mask(u32 mask) const {
	return ioctl(
				arg::IoRequest(I_PIO_CLRMASK),
				arg::IoIntArgument(mask)
				);
}

int Pio::assign(u32 value) const {
	pio_attr_t attr;
	attr.o_pinmask = value;
	attr.o_flags = ASSIGN;
	return ioctl(
				arg::IoRequest(I_PIO_SETATTR),
				arg::IoArgument(&attr)
				);
}

u32 Pio::get_value() const {
	u32 value = 0;
	ioctl(
				arg::IoRequest(I_PIO_GET),
				arg::IoArgument(&value)
				);
	return value;
}

int Pio::set_value(unsigned int value) const {
	return ioctl(
				arg::IoRequest(I_PIO_SET),
				arg::IoIntArgument(value)
				);
}

