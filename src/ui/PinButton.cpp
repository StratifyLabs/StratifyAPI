/* Copyright 2016 tgil All Rights Reserved */

#include "ui/PinButton.hpp"
using namespace ui;
using namespace sys;
using namespace hal;

PinButton::PinButton(int port, int pin, bool active_value) : Pin(port, pin) {
	m_active_value = active_value;
}

bool PinButton::get_is_active() const {
	return Pin::get_value() == m_active_value;
}
