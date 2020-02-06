/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Progress.hpp"


using namespace ux;

Progress::Progress(){
	m_value = 0;
	m_maximum = 100;
}


void Progress::handle_event(const ux::Event & event){
	//change the state when an event happens in the component
	if( event.type() == ux::TouchEvent::event_type() ){
		const ux::TouchEvent & touch_event
				= event.reinterpret<ux::TouchEvent>();

		if( (touch_event.id() == ux::TouchEvent::id_released) &&
				contains(touch_event.point()) ){
			redraw();
		}
	}

	Component::handle_event(event);
}
