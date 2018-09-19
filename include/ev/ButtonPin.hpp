/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTONPIN_HPP_
#define UI_BUTTONPIN_HPP_

#include "../hal/Pin.hpp"
#include "Button.hpp"

namespace ui {

#if !defined __link
[[deprecated("Use PinButton")]]
#endif
typedef PinButton ButtonPin;

}

#endif /* UI_BUTTONPIN_HPP_ */
