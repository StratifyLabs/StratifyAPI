/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#include <cstdio>
#include "ui/EventLoop.hpp"
#include "sys.hpp"
#include "draw.hpp"

namespace ui {

EventLoopAttr::EventLoopAttr(){
	m_attr.update_period = 0;
	m_attr.hibernate_timeout = 0;
}

EventLoop::EventLoop(ui::Element & start_element, sgfx::Bitmap & bitmap, sgfx::Bitmap * scratch) {
	// TODO Auto-generated constructor stub
	m_current_element = &start_element;

	//use the full screen
	m_drawing_attr.set_bitmap(bitmap);
	m_drawing_attr.set_scratch(scratch);
	m_drawing_attr.set_point(0,0);
	m_drawing_attr.set_dim(1000, 1000);

	set_refresh_wait_resolution(5000);
	set_hibernate_timeout(0);
	set_period(50);
}

Element * EventLoop::handle_event(Element * current_element, const ui::Event & event, const DrawingAttr & drawing_attr){
	Element * next_element = current_element;
	if( current_element ){
		if( event.type() != Event::NONE ){
			next_element = current_element->handle_event(event, drawing_attr);
		}

		if( next_element != current_element ){
			handle_transition(current_element, next_element, drawing_attr);
		}
	}
	return next_element;
}

void EventLoop::handle_transition(Element * current_element, Element * next_element, const draw::DrawingAttr & drawing_attr){
	//exit the current element
	if( current_element ){
		current_element->handle_event(Event(Event::EXIT, next_element), drawing_attr);
	}

	if( next_element ){
		Animation animation(next_element->animation_attr());

		if( (animation.type() != Animation::NONE) && drawing_attr.scratch() ){
			animation.init(0, next_element, drawing_attr);
			animation.exec();
		}

		//enter the new element
		next_element->handle_event(Event(Event::ENTER), drawing_attr);
	}
}

bool EventLoop::handle_event(const Event & event){
	Element * tmp = m_current_element;
	tmp = handle_event(m_current_element, event, drawing_attr());
	if( tmp != m_current_element ){
		m_current_element = tmp;
		return true;
	}
	return false;
}


void EventLoop::start(Element * element, const draw::DrawingAttr & drawing_attr){
	if( element ){
		element->handle_event(Event(Event::SETUP), drawing_attr);
		if( element->is_redraw_pending() ){
			drawing_attr.bitmap().refresh();
			element->set_redraw_pending(false);
		}
		handle_transition(0, element, drawing_attr);
	}
}

void EventLoop::start(){
	start(m_current_element, drawing_attr());
	m_update_timer.start();
}

void EventLoop::loop(){
	while( m_current_element != 0 ){

		m_drawing_attr.bitmap().wait(refresh_wait_resolution()); //wait until video memory is free to write
		process_events(); //process all events (this will modify the video memory)

		if( update_period() && (m_update_timer.msec() > update_period()) ){
			m_update_timer.restart();
			handle_event(Event(Event::UPDATE));
		}


		if( m_current_element ){
			//update the screen
			if( m_current_element->is_redraw_pending() ){
				m_drawing_attr.bitmap().refresh();
				m_current_element->set_redraw_pending(false);
			} else {
				Timer::wait_msec(10);
			}


			// \todo Check for hibernate time out

			// \todo Check for tick period delay
		}

	}
}


void EventLoop::execute(){
	start();
	loop();
}

} /* namespace ui */
