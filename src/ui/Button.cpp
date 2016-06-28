/* Copyright 2016 tgil All Rights Reserved */

#include <cstdio>

#include "ui/Button.hpp"
using namespace ui;

u32 Button::m_held_duration = 800;

Button::Button(bool active_value){
	m_event_id = Event::NO_BUTTON;
	m_flags.active = active_value;
	m_flags.press_reported = false;
	m_flags.release_reported = true;
	m_flags.held_reported = false;
}

Event Button::event(){

	if( get_pressed() ){
		return Event(Event::BUTTON_PRESSED, this);
	}

	if( get_held(m_held_duration) ){
		return Event(Event::BUTTON_HOLD, this);
	}

	if( get_actuated() ){
		return Event(Event::BUTTON_ACTUATION, this);
	}

	if( get_released() ){
		return Event(Event::BUTTON_RELEASED, this);
	}

	return Event(Event::NONE);
}

void Button::update(void){
	if( get_value() == (bool)m_flags.active ){ //check to see if Pin is in the active state
		m_timer.start(); //start the timer if it hasn't been started
		printf("Start timer for %d %d\n", m_event_id, m_timer.msec());
	} else {
		m_timer.stop(); //stop the timer (if it hasn't been stopped yet)
	}
}

u32 Button::get_duration(){
	u32 value = m_timer.msec();

	//reset the timer so the duration is only returned once after the button is released
	if( m_timer.is_stopped() ){
		m_timer.reset();
	}

	return value;
}

bool Button::get_pressed(){
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

bool Button::get_released(){
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

bool Button::get_actuated(){
	bool ret;
	u32 msec;

	ret = false;
	if( m_flags.actuation_reported == 0 ){
		msec = m_timer.msec();
		//true if time meets minimum and less than maximum and timer is not running
		if( (msec > 5) && (msec < m_held_duration) && (!m_timer.is_running()) ){
			ret = true;
			m_flags.actuation_reported = 1;
		}
	}

	return ret;
}

bool Button::get_held(u32 msec){
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
