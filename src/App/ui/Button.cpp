/* Copyright 2016 tgil All Rights Reserved */

#include <ui/Button.hpp>
using namespace ui;

u32 Button::m_held_duration = 800;

Button::Button(){
	m_event_button = Event::NO_BUTTON;
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
