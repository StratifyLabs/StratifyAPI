//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved




#include <unistd.h>
#include <fcntl.h>
#include "Hal/Eint.hpp"
using namespace Hal;

Eint::Eint(port_t port) : Periph(CORE_PERIPH_EINT, port){}

int Eint::attr(eint_attr_t * attr){
	return ioctl(I_EINT_GETATTR, attr);
}

int Eint::setattr(const eint_attr_t * attr){
	return ioctl(I_EINT_SETATTR, attr);
}

int Eint::setaction(const eint_action_t & action){
	return ioctl(I_EINT_SETACTION, &action);
}

