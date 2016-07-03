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
	Element * ret = current_element;
	if( current_element ){
		if( event.type() != Event::NONE ){
			ret = current_element->handle_event(event, drawing_attr);
		}

		if( ret != current_element ){
			current_element = ret;
			if( current_element ){
				current_element->handle_event(Event(Event::ENTER), drawing_attr);
			}
		}
	}
	return ret;
}

void EventLoop::handle_event(const Event & event){
	Element * tmp = m_current_element;
	tmp = handle_event(m_current_element, event, m_drawing_attr);
	if( tmp != m_current_element ){
		m_current_element = tmp;
		handle_element_changed();
	}
}

void EventLoop::start(){
	if( m_current_element) m_current_element->handle_event(Event(Event::SETUP), drawing_attr());

	if( m_current_element) {
		m_current_element->handle_event(Event(Event::ENTER), drawing_attr());
	}

	m_update_timer.start();
}

void EventLoop::loop(){
	while( m_current_element != 0 ){

			//m_loop_timer.restart();

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

			/*
			msec = m_loop_timer.msec();
			if( msec < period() ){
				printf("wait %ld msec\n", msec);
				Timer::wait_msec(period() - msec);
			}
			*/


		}

}


void EventLoop::execute(){
	start();
	loop();
}

} /* namespace ui */
