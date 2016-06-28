/* Copyright 2016 tgil All Rights Reserved */

#include "ui/ButtonPin.hpp"
using namespace ui;
using namespace sys;
using namespace hal;

ButtonPin::ButtonPin(int port, int pin, bool active) : Pin(port, pin) {}

bool ButtonPin::get_is_active() const {
	return Pin::get_value() == active_value();
}
