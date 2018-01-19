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

	/*! \details This method accesses how long the user must hold the
	 * button in the application before the ui::Event::BUTTON_HOLD is triggered
	 * when calling event()
	 *
	 * @return The duration in milliseconds
	 */
	static u32 held_duration(){ return m_held_duration; }

	/*! \details This method sets the duration of the hold event.
	 */
	static void set_held_duration(u32 duration_ms){ m_held_duration = duration_ms; }

	/*! \details This method returns the max held duration which will effectively disable the
	 * reporting of button holds
	 *
	 *
	 *  \code
	 *  Button::set_held_duration( Button::max_held_duration() );
	 *  \endcode
	 */
	static u32 max_held_duration(){ return (u32)-1; }

	/*! \details This method checks the state of the actuation and then returns
	 * an Event if needed.
	 *
	 * It will report the following events
	 *
	 *  - ui::Event::BUTTON_PRESSED
	 *  - ui::Event::BUTTON_RELEASED
	 *  - ui::Event::BUTTON_ACTUATED
	 *  - ui::Event::BUTTON_HOLD
	 *
	 */
	ui::Event event();

	/*! \details Set the event ID of the button */
	void set_event_id(enum ui::Event::button_id v){ m_event_id = v; }
	/*! \details Access the event ID of the button */
	enum ui::Event::button_id event_id() const { return m_event_id; }

	/*! \details Returns the duration of the last button press.
	 * This method will only return a non-zero value once per button press.
	 */
	virtual u32 get_duration();

	/*! \details Returns true if the button has been pressed. This will
	 * only return true once per button press.
	 *
	 */
	virtual bool get_pressed();

	/*! \details Returns true if the button has been released.
	 * This will only return true one time for each button press.
	 */
	virtual bool get_released();

	/*! \details Returns true if the button was pressed then released
	 * This will return true only once per button press.
	 */
	virtual bool get_actuated();

	/*! \details Returns true if the button has been held for the specified duration
	 * This will only return true once per button press.
	 *
	 *
	 * @param msec The number of milliseconds the button has been held for
	 */
	virtual bool get_held(u32 msec);

	/*! \details This will update the state of the button. This method
	 * should be called periodically.
	 */
	virtual void update();

	/*! \details Reset the state of the button */
	virtual void reset();

	/*! \details Returns true if the button is currently in the active state.
	 * The active state is updated each time Button::update() is called.
	 *
	 */
	bool is_active(){ return m_timer.is_running(); }

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
		unsigned duration_reported:1;
	};

	struct button_flags m_flags;

};

};

#endif /* UI_BUTTON_HPP_ */
