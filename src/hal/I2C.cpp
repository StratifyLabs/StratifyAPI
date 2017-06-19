//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "hal/I2C.hpp"
using namespace hal;


I2C::I2C(port_t port) : Periph(CORE_PERIPH_I2C, port){}

int I2C::get_attr(i2c_attr_t & attr) const {
	return ioctl(I_I2C_GETATTR, &attr);
}

int I2C::set_attr(const i2c_attr_t & attr) const {
	return ioctl(I_I2C_SETATTR, &attr);
}

int I2C::setup(const i2c_reqattr_t & setup) const {
	return ioctl(I_I2C_SETUP, &setup);
}

int I2C::setup_slave(const i2c_slave_setup_t & setup) const {
	return ioctl(I_I2C_SLAVE_SETUP, &setup);
}

int I2C::reset() const {
	return ioctl(I_I2C_RESET);
}

int I2C::get_err() const {
	return ioctl(I_I2C_GETERR);
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
