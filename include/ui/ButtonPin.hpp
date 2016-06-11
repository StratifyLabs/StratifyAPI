/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTONPIN_HPP_
#define UI_BUTTONPIN_HPP_

#include "../sys/Timer.hpp"
#include "../hal/Pin.hpp"
#include "Button.hpp"

namespace ui {

class ButtonPin : public Button, public hal::Pin {
public:

	ButtonPin(int port, int pin, bool active = active_high());

	u32 get_duration();
	bool get_pressed();
	bool get_released();
	bool get_actuated();
	bool get_held(u32 msec);
	void update();

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

#endif /* UI_BUTTONPIN_HPP_ */
