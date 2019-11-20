//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "hal/I2C.hpp"
using namespace hal;


I2C::I2C(port_t port) : Periph(CORE_PERIPH_I2C, port){}

int I2C::prepare(u8 slave_addr, u32 o_flags) const {
	i2c_attr_t attr;
	attr.o_flags = o_flags;
	attr.slave_addr[0].addr8[0] = slave_addr;
	return set_attributes(attr);
}


int I2C::reset() const {
	i2c_attr_t attr;
	attr.o_flags = I2C::RESET;
	return set_attributes(attr);
}

int I2C::get_error() const {
	i2c_info_t info;
	get_info(info);
	return info.err;
}


int I2C::read(const arg::Location & location, u8 & reg){
	int ret;
	ret = read(
				location,
				arg::DestinationBuffer(&reg),
				arg::Size(1)
				);
	return ret;
}

int I2C::write(const arg::Location & location, u8 reg){
	int ret;
	ret = write(
				location,
				arg::SourceBuffer(&reg),
				arg::Size(1)
				);
	return ret;
}


int I2C::set(const arg::Location & location, int bit, bool high){
	int ret;
	u8 tmp;
	ret = read(location, tmp);
	if( ret < 0 ){
		return ret;
	}
	if ( high == true ){
		tmp |= (1<<bit);
	} else {
		tmp &= ~(1<<bit);
	}
	return write(location, tmp);
}
