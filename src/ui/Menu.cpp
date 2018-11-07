/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#include "draw.hpp"
#include "sgfx.hpp"

#include "ui/Menu.hpp"
using namespace ui;

Menu::Menu() {
	// TODO Auto-generated constructor stub
	m_current = 0;
	m_animation.set_type(AnimationAttr::PUSH_LEFT);
	m_animation.set_path(AnimationAttr::SQUARED);
	m_animation.set_drawing_motion_total(1000);
}

Element * Menu::handle_event(const Event & event, const DrawingAttr & attr){
	Element * next = 0;
	Element * previous = 0;
	u8 type;
	switch(event.type()){
		default: break;
		case Event::SETUP:
			Element::handle_event(event, attr);
			break;


		case Event::MENU_BACK:

			previous = event.element();
			if( previous ){
				previous->handle_event(Event(Event::EXIT, m_current), attr);
			}

			current().handle_event(event, attr);
			next = current().parent();
			if( next ){
				//start the animation to the left
				m_animation.set_type(AnimationAttr::PUSH_RIGHT);
				m_animation.init(0, next, attr);
				m_animation.exec();
				set_current(*next);
				current().handle_event(Event(Event::ENTER), attr);
				return this;
			} else {
				//if there is no parent, then let the element handle the menu back event
				current().handle_event(event, attr);
			}
			break;

		case Event::MENU_ACTUATED:

			//exit the previous element
			previous = event.element();
			if( previous ){
				previous->handle_event(Event(Event::EXIT, m_current), attr);
			}

			//execute the animation
			type = ((LinkedElement*)next)->animation_type();
			if( type != Animation::NONE ){
				m_animation.set_type( type );
				m_animation.init(0, m_current, attr);
				m_animation.exec();
			}
			current().handle_event(Event(Event::ENTER), attr);
			return this;

		case Event::ENTER:
			m_animation.set_type( current().animation_type() );
			m_animation.set_frame_delay( current().animation_frame_delay() );
			m_animation.set_path( current().animation_path() );
			m_animation.init(0, m_current, attr);
			m_animation.exec();
			break;
	}

	if( m_current ){
		previous = m_current;
		next = current().handle_event(event, attr);
		if( current().is_redraw_pending() ){
			set_redraw_pending();
			current().set_redraw_pending(false);
		}
		if( next && (next != previous) ){
			printf("Actuate Menu\n");
			set_current(*next);
			Menu::handle_event(Event(Event::MENU_ACTUATED, previous), attr);
		}
	}

	return this;

}

void Menu::draw(const DrawingAttr & attr){
	if( m_current == 0 ){
		return Element::draw(attr);
	}
	return current().draw(attr);
}
