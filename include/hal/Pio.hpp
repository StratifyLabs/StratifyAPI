/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_PIO_HPP_
#define SAPI_PIO_HPP_

#include <sos/dev/pio.h>
#include "Periph.hpp"

namespace hal {


/*! \brief Pin Input/Output Class
 * \details This class controls pin input/output.
 *
 * \sa Hal::Pin
 *
 * Example:
 * \code
 *
 * #include <stfy/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 *
 *	//This allocates a PIO object but does not affect HW state
 * 	Pio pio(2); //use pio port 2
 *
 * 	//This opens the port and sets the attributes -- same as pio.open(); pio.set_attr(1<<10, PIO_MODE_OUTPUT);
 * 	pio.init(1<<10, PIO_MODE_OUTPUT);
 *
 * 	//Bits can be manipulated using clrmask() and setmask()
 * 	pio.clrmask(1<<10); //clear bit 10 of port 2
 * 	pio.setmask(1<<10); //set bit 10
 *
 *	//the pin can be reconfigured as an input
 * 	pio.set_attr(1<<10, PIO_MODE_INPUT | PIO_MODE_PULLDOWN); //set as an input with internal pulldown
 *
 * 	//get() is used to read the port
 * 	if( pio.get() & (1<<10) ){
 *		//bit 10 is set
 * 	}
 *
 * 	//This closes the port for all instances (ports are closed automatically when program exits)
 * 	pio.close();
 *
 * }
 *
 * \endcode
 */
class Pio : public Periph<pio_info_t, pio_attr_t, 'p'> {
public:
	Pio(port_t port);

	enum {
		FLAG_SET_INPUT /*! Input flag*/ = PIO_FLAG_SET_INPUT,
		FLAG_SET_OUTPUT /*! Output flag */ = PIO_FLAG_SET_OUTPUT,
		FLAG_IS_PULLUP /*! Use the built-in pullup */ = PIO_FLAG_IS_PULLUP,
		FLAG_IS_PULLDOWN /*! Use the built-in pull-down */ = PIO_FLAG_IS_PULLDOWN,
		FLAG_IS_REPEATER /*! Use the built-in repeater function */ = PIO_FLAG_IS_REPEATER,
		FLAG_IS_FLOAT /*! Leave the pin floating */ = PIO_FLAG_IS_FLOAT,
		FLAG_IS_SPEED_LOW /*! Max speed 2Mhz (stm32f only) */ = PIO_FLAG_IS_SPEED_LOW,
		FLAG_IS_SPEED_MEDIUM /*! Add fast mode slew rate */ = PIO_FLAG_IS_SPEED_MEDIUM,
		FLAG_IS_SPEED_HIGH /*! Max speed 10Mhz (stm32f only) */ = PIO_FLAG_IS_SPEED_HIGH,
		FLAG_IS_SPEED_BLAZING /*! Max speed 50Mhz (stm32f only) */ = PIO_FLAG_IS_SPEED_BLAZING,
		FLAG_IS_OPENDRAIN /*! Configure as open drain */ = PIO_FLAG_IS_OPENDRAIN,
		FLAG_IS_HYSTERESIS /*! Enable hysteresis on pin */ = PIO_FLAG_IS_HYSTERESIS,
		FLAG_IS_DIRONLY /*! Only set input/output (ignore other settings) */ = PIO_FLAG_IS_DIRONLY,
		FLAG_IS_ANALOG /*! Use an analog rather than digital input */ = PIO_FLAG_IS_ANALOG,
		FLAG_IS_INVERT /*! Invert the logic on the pin */ = PIO_FLAG_IS_INVERT,
		FLAG_IS_FILTER /*! Filter noise on pin */ = PIO_FLAG_IS_FILTER,
		FLAG_SET /*! Set the bits in the mask */ = PIO_FLAG_SET,
		FLAG_CLEAR /*! Clear the bits in the mask */ = PIO_FLAG_CLEAR,
		FLAG_ASSIGN /*! Assign the pinmask value to the port */ = PIO_FLAG_ASSIGN
	};

	/*! \details Set the specified pin mask */
	int set_mask(u32 mask) const;
	/*! \details Clear the specified mask */
	int clear_mask(u32 mask) const;

	int assign(u32 mask) const;

	/*! \details Get the value of the port */
	u32 get_value() const;
	u32 value() const { return get_value(); }

	/*! \details Set the value of the port */
	int set_value(unsigned int value);

	int set_attr(u32 o_flags, u32 o_pinmask){
		pio_attr_t attr;
		attr.o_flags;
		attr.o_pinmask;
		return set_attr(attr);
	}

	/*! \details This method opens the port
	 * then sets the attributes using the specified mask and mode.
	 * \code
	 * Pio p(0);
	 * p.init((1<<10)|(1<<5), PIO_MODE_OUTPUT)); //pins 0.5 and 0.10 are outputs
	 * \endcode
	 */
	int init(u32 o_flags, u32 o_pinmask){
		if( open() <  0 ){
			return -1;
		}
		return set_attr(o_flags, o_pinmask);
	}


private:

};

};

#endif /* SAPI_PIO_HPP_ */
