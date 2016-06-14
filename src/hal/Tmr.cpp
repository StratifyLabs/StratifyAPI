//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Tmr.hpp"

using namespace hal;


Tmr::Tmr(port_t port) : Periph(CORE_PERIPH_TMR, port) {}

int Tmr::get_attr(tmr_attr_t * attr){
	return ioctl(I_TMR_GETATTR, attr);
}

int Tmr::set_attr(const tmr_attr_t * attr){
	return ioctl(I_TMR_SETATTR, attr);
}

int Tmr::set_action(const tmr_action_t * action){
	return ioctl(I_TMR_SETACTION, action);
}

int Tmr::on(){
	return ioctl(I_TMR_ON);
}

int Tmr::off(){
	return ioctl(I_TMR_OFF);
}

int Tmr::set_output_compare(const tmr_reqattr_t * req){
	return ioctl(I_TMR_SETOC, req);
}

int Tmr::get_output_compare(tmr_reqattr_t * req){
	return ioctl(I_TMR_GETOC, req);
}

int Tmr::set_input_capture(const tmr_reqattr_t * req){
	return ioctl(I_TMR_SETIC, req);
}

int Tmr::get_input_capture(tmr_reqattr_t * req){
	return ioctl(I_TMR_GETIC, req);}

tmr_sample_t Tmr::get_value(){
	return ioctl(I_TMR_GET);
}

int Tmr::set_value(tmr_sample_t value){
	return ioctl(I_TMR_SET, value);
}


