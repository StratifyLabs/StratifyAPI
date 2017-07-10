//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


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
	return set_attr(attr);
}


int I2C::reset() const {
	i2c_attr_t attr;
	attr.o_flags = I2C::FLAG_RESET;
	return set_attr(attr);
}

int I2C::get_err() const {
	i2c_info_t info;
	get_info(info);
	return info.err;
}


int I2C::read(int loc, u8 & reg){
	int ret;
	ret = read(loc, &reg, 1);
	return ret;
}

int I2C::write(int loc, u8 reg){
	int ret;
	ret = write(loc, &reg, 1);
	return ret;
}


int I2C::set(int loc, int bit, bool high){
	int ret;
	u8 tmp;
	ret = read(loc, tmp);
	if( ret < 0 ){
		return ret;
	}
	if ( high == true ){
		tmp |= (1<<bit);
	} else {
		tmp &= ~(1<<bit);
	}
	return write(loc, tmp);
}
