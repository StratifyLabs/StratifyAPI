/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef PIN_HPP_
#define PIN_HPP_

#include "Pio.hpp"

namespace hal {


/*! \brief Pin class
 * \details This class controls the operation of a single pin.  It works similarly
 * to the Pio class, but the mask is built-in.
 *
 * Here is an example of using the Pin class:
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * Pin pin(1,0); //control pin P1.0
 * //Or if working with the pinmask Pin  pin(1, 1<<0, true);
 *
 * pin.init(Pin::OUTPUT); //initialize as an output
 *
 * //These three all set the pin high
 * pin = true;
 * pin.set();
 *
 * //These turn the pin off
 * pin = false;
 * pin.clear();
 *
 * //now convert to an input
 * pin.set_attr(Pin::INPUT | Pin::PULLUP); //or use Pin::FLOAT, Pin::PULLDOWN, etc
 * //or to init as an input use pin.init(Pin::INPUT);
 *
 *  //now check to see if the value is high or low
 * if( pin.get_value() == true ){
 *  //pin is high
 * } else {
 * 	//pin is low
 * }
 *
 * pin.close(); //close the associated file descriptor (pin keeps its IO properties and state)
 * \endcode
 *
 */
class Pin : public Pio {
public:
	/*! \details Constructs the object with a port/pin combination. */
	Pin(port_t port, u32 pin, bool ismask = false) : Pio(port){
		if( ismask ){
			m_pinmask = pin;
		} else {
			m_pinmask = (1<<pin);
		}
	}

	/*! \details Contructs a new pin object from an mcu_pin_t data structure. */
	Pin(const mcu_pin_t & p) : Pio(p.port) { m_pinmask = 1<<p.pin; }


	using Pio::set_attr;

	/*! \details Initializes the pin with the specified mode.
	 *
	 * @param o_flags The mode to start the pin (e.g., Pin::INPUT | Pin::PULLUP)
	 *
	 * Hardware intialization opens the device for reading/writing and then
	 * sets the device attributes as specified.
	 *
	 * The following is an example of initializing a pin.
	 *
	 * \code
	 * #include <sapi/hal.hpp>
	 *
	 * Pin p(0,1); //Port 0, pin 1
	 *
	 * p.init(Pin::INPUT | Pin::PULLUP);
	 *
	 * if( p.get_value() == 0 ){
	 *	//this means something is driving the pin low
	 * }
	 * \endcode
	 *
	 * \sa open(), set_attr()
	 *
	 */
	int initialize(u32 o_flags){
		return Pio::initialize(o_flags, m_pinmask);
	}

	int init(u32 o_flags){
		return Pio::init(o_flags, m_pinmask);
	}


	/*! \details Initializes the pin as an input.
	 *
	 * @param o_flags Optionally set FLAG_IS_PULLUP, etc
	 * @return Zero on success
	 */
	int set_input(u32 o_flags = 0){
		return init(FLAG_SET_INPUT | o_flags);
	}

	/*! \details Initializes the pin as an input.
	 *
	 * @param o_flags Optionally set FLAG_IS_OPENDRAIN, etc
	 * @return Zero on success
	 */
	int set_output(u32 o_flags = 0){
		return init(FLAG_SET_OUTPUT | o_flags);
	}

	/*! \details Sets the pin attributes.
	 *
	 * @param o_flags The pin mode (e.g., Pin::INPUT)
	 * @return Zero on success
	 *
	 */
	int set_attributes(u32 o_flags) const {
		return Pio::set_attr(o_flags, m_pinmask);
	}

	int set_attr(u32 o_flags){
		return Pio::set_attr(o_flags, m_pinmask);
	}

	/*! \details Assigns a boolean to the pin (true is high, false is low). */
	Pin& operator = (bool on){
		if( on ){
			set_mask(m_pinmask);
		} else {
			clear_mask(m_pinmask);
		}
		return *this;
	}

	/*! \details Returns true if the pin is high and false if it is low.
	 *
	 * This allows the following code to work:
	 * \code
	 * Pin button;
	 *
	 * if( button ){
	 *   printf("Button is logic high\n");
	 * } else {
	 *   printf("Button is logic low\n");
	 * }
	 * \endcode
	 *
	 */
	operator bool() const { return get_value(); }

	/*! \details Assigns a boolean to the pin.
	 *
	 * @param value If true, sets the pin high
	 */
	void set_value(bool value) const {
		if( value ){
			set_mask(m_pinmask);
		} else {
			clear_mask(m_pinmask);
		}
	}


	/*! \details Gets the value of the pin (true is high, false is low). */
	bool get_value() const { return (Pio::get_value() & m_pinmask) != 0; }


	/*! \details Sets the pin high (assign value 1) */
	int set() const { return set_mask(m_pinmask); }

	/*! \details Clear the pin low (assign value 0) */
	int clear() const { return clear_mask(m_pinmask); }

	/*! \details Accesses the pin's associated Pio pinmask. */
	u32 pinmask() const { return m_pinmask; }

	static bool is_floating(mcu_pin_t pin){
		Pin p(pin);
		p.set_input(Pin::IS_PULLUP);
		if( p == false ){ return false; }
		p.set_attr(Pin::IS_PULLDOWN);
		if( p == true ){ return false; }
		return true;
	}

private:
	u32 m_pinmask;
};

}

#endif /* PIN_HPP_ */
