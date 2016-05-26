//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved




#include <unistd.h>
#include <fcntl.h>
#include <hal/Eint.hpp>
using namespace hal;

Eint::Eint(port_t port) : Periph(CORE_PERIPH_EINT, port){}

int Eint::get_attr(eint_attr_t * attr){
	return ioctl(I_EINT_GETATTR, attr);
}

int Eint::set_attr(const eint_attr_t * attr){
	return ioctl(I_EINT_SETATTR, attr);
}

int Eint::set_action(const eint_action_t & action){
	return ioctl(I_EINT_SETACTION, &action);
}

