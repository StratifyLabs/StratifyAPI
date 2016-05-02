//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "Hal/I2C.hpp"
using namespace Hal;


I2C::I2C(port_t port) : Periph(CORE_PERIPH_I2C, port){}

int I2C::attr(i2c_attr_t * attr){
	return ioctl(I_I2C_GETATTR, attr);
}

int I2C::setattr(const i2c_attr_t * attr){
	return ioctl(I_I2C_SETATTR, attr);
}

int I2C::setup(const i2c_reqattr_t * req){
	return ioctl(I_I2C_SETUP, req);
}

int I2C::geterr(void){
	return ioctl(I_I2C_GETERR);
}


int I2C::read(int loc, uint8_t & reg){
	int ret;
	ret = read(loc, &reg, 1);
	return ret;
}

int I2C::write(int loc, uint8_t reg){
	int ret;
	ret = write(loc, &reg, 1);
	return ret;
}


int I2C::set(int loc, int bit, bool high){
	int ret;
	uint8_t tmp;
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
