/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#include <cstdio>
#include "sys/requests.h"
#include "ev/EventLoop.hpp"
#include "sys.hpp"

namespace ev {

EventLoopAttributes::EventLoopAttributes(){
	m_attr.update_period_usec = 0;
	m_attr.hibernation_threshold_msec = 0xffff;
	m_attr.period_usec = 10000UL;
	m_attr.refresh_wait_resolution_usec = 5000;
}


EventLoop::EventLoop(EventHandler & start_event_handler){
	m_current_event_handler = &start_event_handler;
}


EventHandler * EventLoop::handle_event(EventHandler * current_event_handler, const Event & event, EventLoop * event_loop){
	EventHandler * next_event_handler = current_event_handler;
	if( current_event_handler ){
		if( event.type() != Event::NONE ){
			current_event_handler->set_event_loop(event_loop);
			next_event_handler = current_event_handler->handle_event(event);
		}

		if( next_event_handler && (next_event_handler != current_event_handler) ){
			next_event_handler->set_event_loop(event_loop);
			handle_transition(current_event_handler, next_event_handler);
		}
	}
	return next_event_handler;
}

void EventLoop::handle_transition(EventHandler * current_event_handler, EventHandler * next_event_handler){
	//exit the current element
	if( current_event_handler ){
		current_event_handler->handle_event(Event(Event::EXIT, next_event_handler));
	}

	if( next_event_handler ){
		//enter the new element
		next_event_handler->handle_event(Event(Event::ENTER));
	}
}

bool EventLoop::handle_event(const Event & event){
	EventHandler * tmp = m_current_event_handler;
	tmp = handle_event(m_current_event_handler, event, this);
	if( tmp != m_current_event_handler ){
		if( tmp == 0 ){
			tmp = catch_null_handler(m_current_event_handler);
			if( tmp != 0 ){
				tmp->set_event_loop(this);
				handle_transition(m_current_event_handler, tmp);
			}
		}
		m_current_event_handler = tmp;
		return true;
	}
	return false;
}


void EventLoop::start(EventHandler * event_handler){
	if( event_handler ){
		event_handler->handle_event(Event(Event::SETUP));
		handle_transition(0, event_handler);
	}
}

void EventLoop::start(){
	if( m_current_event_handler ){
		m_current_event_handler->set_event_loop(this);
	}
	start(m_current_event_handler);
	m_update_timer.start();
}

void EventLoop::loop(){
	while( current_event_handler() != 0 ){
		m_loop_timer.restart();
		process_events(); //process all events
		check_loop_for_update();
		check_loop_for_hibernate();
	}
}


void EventLoop::execute(){
	start();
	loop();
}

void EventLoop::check_loop_for_update(){
	if( update_period().microseconds() && ((update_timer() >= update_period()) || (update_period() >= hibernation_threshold())) ){
		m_update_timer.restart();
		handle_event(Event(Event::UPDATE));
	}
}


void EventLoop::check_loop_for_hibernate(){
	if( update_period() >= hibernation_threshold() ){
		sapi_request_hibernate_t request;
		request.update_period_milliseconds = update_period().milliseconds();
		request.loop_period_milliseconds = period().milliseconds();
		if( Sys::request(
				 Sys::KernelRequest(SAPI_REQUEST_HIBERNATE),
				 Sys::KernelArgument(&request)
				 ) < 0 ){
			Sys::hibernate(
						chrono::Milliseconds((update_period().milliseconds() + 500)/ 1000)
						);
		}
	} else {
		s32 us_remaining;
		us_remaining = period().microseconds() - loop_timer().microseconds();
		if( us_remaining > 0 ){
			chrono::wait(chrono::Microseconds(us_remaining));
		}
	}
}

} /* namespace ev */
