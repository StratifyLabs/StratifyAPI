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
	animation.set_type(AnimationAttr::PUSH_LEFT);
	animation.set_path(AnimationAttr::SQUARED);
	animation.set_drawing_motion_total(1000);
}

void Menu::set_animation_type(u8 v){ m_current->set_animation_type(v); }
u8 Menu::animation_type() const{ return m_current->animation_type(); }


Element * Menu::handle_event(const Event & event, const DrawingAttr & attr){
	Element * next = 0;
	u8 type;
	switch(event.type()){
	default: break;
	case Event::SETUP:
		Element::handle_event(event, attr);
		break;


	case Event::MENU_BACK:
		next = current().parent();
		if( next ){
			//start the animation to the left
			animation.set_type(AnimationAttr::PUSH_RIGHT);
			animation.init(0, next, attr);
			animation.exec();
			set_current(*next);
			current().handle_event(Event(Event::ENTER), attr);
			return this;
		} else {
			//if there is no parent, then let the list handle the menu back event
			current().handle_event(event, attr);
		}
		break;

	case Event::LIST_ACTUATE:
		next = current().handle_event(event, attr);
		if( next && (next != m_current) ){
			type = ((ElementLinked*)next)->animation_type();
			animation.set_type( type );
			animation.init(0, next, attr);
			animation.exec();
			set_current(*next);
			current().handle_event(Event(Event::ENTER), attr);
		}
		return this;

	case Event::ENTER:
		type = current().animation_type();
		animation.set_type( type );
		animation.init(0, m_current, attr);
		animation.exec();
		break;
	}

	if( m_current ){
		current().handle_event(event, attr);
		if( current().is_redraw_pending() ){
			set_redraw_pending();
			current().set_redraw_pending(false);
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
