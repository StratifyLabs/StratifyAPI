//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include "hal/Pio.hpp"
using namespace hal;

Pio::Pio(port_t port) : Periph(CORE_PERIPH_PIO, port){}



int Pio::set_mask(u32 mask) const {
	return ioctl(
				IoRequest(I_PIO_SETMASK),
				IoIntArgument(mask)
				);
}

int Pio::clear_mask(u32 mask) const {
	return ioctl(
				IoRequest(I_PIO_CLRMASK),
				IoIntArgument(mask)
				);
}

int Pio::assign(u32 value) const {
	pio_attr_t attr;
	attr.o_pinmask = value;
	attr.o_flags = ASSIGN;
	return ioctl(
				IoRequest(I_PIO_SETATTR),
				IoArgument(&attr)
				);
}

u32 Pio::get_value() const {
	u32 value = 0;
	ioctl(
				IoRequest(I_PIO_GET),
				IoArgument(&value)
				);
	return value;
}

int Pio::set_value(unsigned int value) const {
	return ioctl(
				IoRequest(I_PIO_SET),
				IoIntArgument(value)
				);
}

