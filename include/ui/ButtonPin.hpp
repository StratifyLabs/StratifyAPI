/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTONPIN_HPP_
#define UI_BUTTONPIN_HPP_

#include "../hal/Pin.hpp"
#include "Button.hpp"

namespace ui {

/*! \brief Button Pin Class
 * \details This class implements a button using a hal::Pin object.
 */
class ButtonPin : public Button, public hal::Pin {
public:

	/*! \details Construct a new ButtonPin
	 *
	 * The pin must be initialized using Pin::init()
	 *
	 * @param port The port associated with the pin
	 * @param pin The pin number on the port for the button input
	 * @param active_value true for active high and false for active low
	 */
	ButtonPin(int port, int pin, bool active_value = true);

	/*! \details This method accesses the active value.
	 *
	 * @return True for active high and false for active low.
	 */
	bool active_value() const { return m_active_value; }

private:
	bool get_is_active() const;
	bool m_active_value;

};

}

#endif /* UI_BUTTONPIN_HPP_ */
