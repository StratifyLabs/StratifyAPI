/* Copyright 2016 tgil All Rights Reserved */

#include "ui/ButtonPin.hpp"
using namespace ui;
using namespace sys;
using namespace hal;

ButtonPin::ButtonPin(int port, int pin, bool active) : Pin(port, pin) {
	// TODO Auto-generated constructor stub
	m_flags.active = active;
	m_flags.press_reported = false;
	m_flags.release_reported = true;
	m_flags.held_reported = false;
}

void ButtonPin::update(void){
	if( value() == (bool)m_flags.active ){ //check to see if Pin is in the active state
		m_timer.start(); //start the timer if it hasn't been started
	} else {
		m_timer.stop(); //stop the timer (if it hasn't been stopped yet)
	}
}

u32 ButtonPin::get_duration(){
	u32 value = m_timer.msec();

	//reset the timer so the duration is only returned once after the button is released
	if( m_timer.is_stopped() ){
		m_timer.reset();
	}

	return value;
}

bool ButtonPin::get_pressed(){
	if( m_flags.press_reported == 0 ){
		if( m_timer.is_running() ){
			m_flags.press_reported = 1;
			m_flags.release_reported = 0;
			m_flags.held_reported = 0;
			m_flags.actuation_reported = 0;
			return true;
		}
	}
	return false;
}

bool ButtonPin::get_released(){
	if( m_flags.release_reported == 0 ){
		if( !m_timer.is_running() ){
			m_flags.press_reported = 0;
			m_flags.release_reported = 1;
			m_flags.held_reported = 0;
			return true;
		}
	}
	return false;
}

bool ButtonPin::get_actuated(){
	bool ret;
	u32 msec;

	ret = false;
	if( m_flags.actuation_reported == 0 ){
		msec = m_timer.msec();
		//true if time meets minimum and less than maximum and timer is not running
		if( (msec > 10) && (msec < m_held_duration) && (!m_timer.is_running()) ){
			ret = true;
			m_flags.actuation_reported = 1;
		}
	}

	return ret;
}

bool ButtonPin::get_held(u32 msec){
	bool ret = false;

	if( m_flags.held_reported == 0 ){
		//true if timer is greater than a maximum and held has not been reported
		if ( (m_timer.msec() >= msec) && (m_timer.is_running()) ){
			ret = true;
			m_flags.held_reported = 1;
		}
	}
	return ret;
}
