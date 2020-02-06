/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */

#include "ev/Button.hpp"

using namespace ev;

ev::Event Button::get_event(){

	//update the state of the button
	update();

	if( get_pressed() ){
		return ev::Event(ev::Event::BUTTON_PRESSED, this);
	}

	if( get_held() ){
		return ev::Event(ev::Event::BUTTON_HELD, this);
	}

	if( get_released() ){
		return ev::Event(ev::Event::BUTTON_RELEASED, this);
	}

	if( get_actuated() ){
		return ev::Event(ev::Event::BUTTON_ACTUATED, this);
	}

	return ev::Event(ev::Event::NONE);
}

void Button::reset(){}

void Button::update(){}

