/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */

#ifndef SAPI_EV_PINBUTTON_HPP_
#define SAPI_EV_PINBUTTON_HPP_

#include "../hal/Pin.hpp"
#include "../chrono/Timer.hpp"
#include "Button.hpp"

namespace ev {

/*! \brief Pin Button Class
 * \details This class implements a button using a hal::Pin object.
 *
 * This class samples the pin during the ui::EventLoop. If the ui::EventLoop
 * is sluggish at times, the button presses might not be responsive (or may be missed).
 *
 * If this is the case, it is better to use DeviceButton because
 * DeviceButton samples in the the background and will queue button
 * presses if the ui::EventLoop is being slow.
 *
 */
class PinButton : public Button, public hal::Pin {
public:

	/*! \details Construct a new ButtonPin
	 *
	 * The pin must be initialized using Pin::init()
	 *
	 */
	PinButton(
			Port port,
			Number pin,
			IsActiveHigh is_active_high = IsActiveHigh(true)
			);

	/*! \details Accesses how long the user must hold the
	  * button in the application before the ui::Event::BUTTON_HELD is triggered
	  * when calling event()
	  *
	  * @return The duration in milliseconds
	  */
	static chrono::MicroTime held_duration(){ return m_held_duration; }

	/*! \details Sets the duration of the hold event.
	  */
	static void set_held_duration(const chrono::MicroTime & duration){ m_held_duration = duration; }

	/*! \details Sets the duration of a button press before a actuation is trigger.
	  *
	  * This can be used to debounce the button.
	  *
	  */
	static void set_actuation_duration(const chrono::MicroTime & duration){ m_actuation_duration = duration; }

	/*! \details Returns the actuation duration (amount of time button is pressed
	  * before triggering an acutation.
	  *
	  */
	static const chrono::MicroTime & actuation_duration(){ return m_actuation_duration; }

	/*! \details Accesses the active value.
	 *
	 * @return True for active high and false for active low.
	 */
	bool active_value() const { return m_active_value; }

	/*! \details Sets the ID of the button for Events */
	void set_event_id(enum ev::Event::button_id v){ m_event_id = v; }

	//methods documented at ui::Button
	enum ev::Event::button_id event_id() const { return m_event_id; }
	chrono::MicroTime get_duration();
	bool get_pressed();
	bool get_released();
	bool get_actuated();
	bool get_held();
	bool is_active() const { return get_is_active(); }
	void reset();


protected:
	void update();
	virtual bool get_is_active() const;

private:

	/*! \cond */
	static chrono::MicroTime m_held_duration;
	static chrono::MicroTime m_actuation_duration;
	enum ev::Event::button_id m_event_id;

	bool m_active_value;

	chrono::Timer m_timer;

	struct button_flags {
		unsigned press_reported:1;
		unsigned release_reported:1;
		unsigned held_reported:1;
		unsigned actuation_reported:1;
		unsigned duration_reported:1;
	};

	struct button_flags m_flags;
	/*! \endcond */

};

}

#endif /* SAPI_EV_PINBUTTON_HPP_ */
