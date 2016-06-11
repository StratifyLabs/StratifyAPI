/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTON_HPP_
#define UI_BUTTON_HPP_


#include <mcu/types.h>

#include "Event.hpp"

namespace ui {

class Button {
public:
	Button();

	static u32 held_duration(){ return m_held_duration; }
	static void set_held_duration(u32 v){ m_held_duration = v; }

	static bool active_high(){ return true; }
	static bool active_low(){ return false; }

	/*! \details This method checks the state of the actuation and then returns
	 * an Event if needed.
	 *
	 * @return
	 */
	ui::Event event();

	void set_event_id(enum ui::Event::button_id v){ m_event_button = v; }
	enum ui::Event::button_id event_id() const { return m_event_button; }

	virtual u32 get_duration() = 0;
	virtual bool get_pressed() = 0;
	virtual bool get_released() = 0;
	virtual bool get_actuated() = 0;
	virtual bool get_held(u32 msec) = 0;
	virtual void update(){}

protected:
	static u32 m_held_duration;
	enum ui::Event::button_id m_event_button;

private:

};

};

#endif /* UI_BUTTON_HPP_ */
