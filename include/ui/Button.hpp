/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTON_HPP_
#define UI_BUTTON_HPP_


#include <mcu/types.h>

#include "../sys/Timer.hpp"
#include "Event.hpp"

namespace ui {

class Button {
public:
	Button(bool active_value = true);

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

	void set_event_id(enum ui::Event::button_id v){ m_event_id = v; }
	enum ui::Event::button_id event_id() const { return m_event_id; }

	virtual u32 get_duration();
	virtual bool get_pressed();
	virtual bool get_released();
	virtual bool get_actuated();
	virtual bool get_held(u32 msec);
	virtual void update();

	bool active_value() const { return m_flags.active != 0; }

protected:

	virtual bool get_value() const = 0;

	static u32 m_held_duration;
	enum ui::Event::button_id m_event_id;

private:

	sys::Timer m_timer;

	struct button_flags {
		unsigned active:1;
		unsigned press_reported:1;
		unsigned release_reported:1;
		unsigned held_reported:1;
		unsigned actuation_reported:1;
	};

	struct button_flags m_flags;

};

};

#endif /* UI_BUTTON_HPP_ */
