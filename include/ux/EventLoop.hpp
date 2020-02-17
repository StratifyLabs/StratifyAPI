/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_EVENTLOOP_HPP
#define SAPI_UX_EVENTLOOP_HPP

#include "Layout.hpp"
#include "../hal/Display.hpp"
#include "../chrono/Timer.hpp"
#include "../sgfx/Theme.hpp"

namespace ux {

class EventLoop {
public:
	EventLoop();

	int loop(
			Layout & layout,
			const sgfx::Theme & theme,
			hal::Display & display
			);

	const chrono::Timer & timer(){
		return m_timer;
	}

	void handle_event(const Event & event);
	/*! \details Process events should be implemented
		* to call handle_event() for each
		* event in the system that happens.
		*
		*/
	virtual void process_events() = 0;

	void trigger_event(const Event & event){
		handle_event(event);
	}


	void set_update_period(
			const chrono::MicroTime & duration
			){
		m_update_period = duration;
	}

	const sgfx::Theme * theme() const {
		return m_theme;
	}

	hal::Display * display(){
		return m_display;
	}

	const hal::Display * display() const {
		return m_display;
	}

private:
	chrono::Timer m_timer;
	chrono::Timer m_update_timer;
	chrono::MicroTime m_update_period;
	Layout * m_layout;
	hal::Display * m_display;
	const sgfx::Theme * m_theme;

	void process_update_event();
};

}

#endif // SAPI_UX_EVENTLOOP_HPP
