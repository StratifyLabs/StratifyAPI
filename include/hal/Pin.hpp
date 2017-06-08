/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

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
 * #include <stfy/hal.hpp>
 *
 * Pin pin(1,0); //control pin P1.0
 *  //Or if working with the pinmask Pin  pin(1, 1<<0, true);
 *
 * pin.init(Pin::OUTPUT); //initialize as an output
 *
 *  //These three all set the pin high
 * pin = true;
 * pin.set();
 *
 *  //These turn the pin off
 * pin = false;
 * pin.clr();
 *
 *  //now convert to an input
 * pin.set_attr(Pin::INPUT | Pin::PULLUP); //or use Pin::FLOAT, Pin::PULLDOWN, etc
 *  //or to init as an input use pin.init(Pin::INPUT);
 *
 *  //now check to see if the value is high or low
 *  if( pin.value() == true ){
 *  	//pin is high
 *  } else {
 *  	//pin is low
 *  }
 *
 *  pin.close(); //close the associated file descriptor (pin keeps its IO properties and state)
 *
 * \endcode
 *
 */
class Pin : public Pio {
public:
	/*! \details Initialize the object with a port/pin combination. */
	Pin(port_t port, uint32_t pin, bool ismask = false) : Pio(port){
		if( ismask ){
			m_pinmask = pin;
		} else {
			m_pinmask = (1<<pin);
		}
	}

	Pin(pio_t p) : Pio(p.port) { m_pinmask = 1<<p.pin; }

	using Pio::set_attr;

	/*! \details Initialize the pin with the specified mode */
	inline int init(int mode){
		return Pio::init(m_pinmask, mode);
	}

	/*! \details Set the pin attributes */
	inline int set_attr(int mode){
		return Pio::set_attr(m_pinmask, mode);
	}

	/*! \details Assign a boolean to the pin (true is high, false is low) */
	Pin& operator = (bool on){
		if( on ){
			set_mask(m_pinmask);
		} else {
			clear_mask(m_pinmask);
		}
		return *this;
	}

	void set_value(bool v){ *this = v; }


	/*! \details Get the value of the pin (true is high, false is low) */
	inline bool get_value() const { return (Pio::get_value() & m_pinmask) != 0; }
	inline bool value(){ return (Pio::get_value() & m_pinmask) != 0; }


	/*! \details Set the pin high (assign value 1) */
	inline int set(){ return set_mask(m_pinmask); }
	/*! \details Clear the pin low (assign value 0) */
	inline int clr(){ return clear_mask(m_pinmask); }
	inline int clear(){ return clear_mask(m_pinmask); }

	pio_sample_t pinmask() const { return m_pinmask; }

private:
	pio_sample_t m_pinmask;
};

};

#endif /* PIN_HPP_ */
