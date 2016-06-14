//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Pwm.hpp"

using namespace hal;

Pwm::Pwm(port_t port) : Periph(CORE_PERIPH_PWM, port){}

int Pwm::get_attr(pwm_attr_t * attr){
	return ioctl(I_PWM_GETATTR, attr);
}

int Pwm::set_attr(const pwm_attr_t * attr){
	return ioctl(I_PWM_SETATTR, attr);
}

int Pwm::set_duty_cycle(const pwm_reqattr_t * req){
	return ioctl(I_PWM_SET, req);
}


