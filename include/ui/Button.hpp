/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTON_HPP_
#define UI_BUTTON_HPP_


#include <mcu/types.h>

#include "../sys/Timer.hpp"
#include "Event.hpp"

namespace ui {

/*! \brief Button Class
 * \details This class implements a button
 * that can be pressed by the user.  It implements the timing
 * for button actuations, button holds, presses and releases.  It
 * is an abstract class where the inheriting classes must implement
 * the ui::Button::get_value() method which returns the value of input.
 */
class Button {
public:

	/*! \details This constructs a new button
	 */
	Button();

	/*! \details This method access how long the user must hold the
	 * button in the application before the ui::Event::BUTTON_HOLD is triggered
	 * when calling event()
	 *
	 * @return The duration in milliseconds
	 */
	static u32 held_duration(){ return m_held_duration; }

	/*! \details This method sets the duration of the hold event.
	 */
	static void set_held_duration(u32 v){ m_held_duration = v; }

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

	virtual void reset();

protected:

	virtual bool get_is_active() const = 0;

	static u32 m_held_duration;
	enum ui::Event::button_id m_event_id;

private:

	sys::Timer m_timer;

	struct button_flags {
		unsigned press_reported:1;
		unsigned release_reported:1;
		unsigned held_reported:1;
		unsigned actuation_reported:1;
	};

	struct button_flags m_flags;

};

};

#endif /* UI_BUTTON_HPP_ */
