/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTONPIN_HPP_
#define UI_BUTTONPIN_HPP_

#include "../hal/Pin.hpp"
#include "Button.hpp"

namespace ui {

class ButtonPin : public Button, public hal::Pin {
public:

	ButtonPin(int port, int pin, bool active = active_high());

protected:
	virtual bool get_is_active() const;

private:

};

};

#endif /* UI_BUTTONPIN_HPP_ */
