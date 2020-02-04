/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_HAL_PIO_HPP_
#define SAPI_HAL_PIO_HPP_

#include <sos/dev/pio.h>
#include "Periph.hpp"

namespace hal {

class PioFlags {
public:
	enum flags {
		SET_INPUT /*! Sets the pinmask as input */ = PIO_FLAG_SET_INPUT,
		SET_OUTPUT /*! Sets the pinmask as output */ = PIO_FLAG_SET_OUTPUT,
		IS_PULLUP /*! When setting as input, enables the pullup */ = PIO_FLAG_IS_PULLUP,
		IS_PULLDOWN /*! When setting as input, enables the pulldown */ = PIO_FLAG_IS_PULLDOWN,
		IS_REPEATER /*! When setting as input, enables repeater mode (if available) */ = PIO_FLAG_IS_REPEATER,
		IS_FLOAT /*! When setting as input, enables floating mode (no pullup or pulldown) */ = PIO_FLAG_IS_FLOAT,
		IS_SPEED_LOW /*! When setting as output, enables low speed mode */ = PIO_FLAG_IS_SPEED_LOW,
		IS_SPEED_MEDIUM /*! When setting as output, enables medium speed mode */ = PIO_FLAG_IS_SPEED_MEDIUM,
		IS_SPEED_HIGH /*! When setting as output, enables high speed mode */ = PIO_FLAG_IS_SPEED_HIGH,
		IS_SPEED_BLAZING /*! When setting as output, enables blazing speed mode */ = PIO_FLAG_IS_SPEED_BLAZING,
		IS_OPENDRAIN /*! When setting as output, enables open drain mode */ = PIO_FLAG_IS_OPENDRAIN,
		IS_HYSTERESIS /*! When setting as input, enables hysteresis */ = PIO_FLAG_IS_HYSTERESIS,
		IS_DIRONLY /*! When set, only affects the direction withouth setting other attributes */ = PIO_FLAG_IS_DIRONLY,
		IS_ANALOG /*! When setting as input, sets pin as analog mode */ = PIO_FLAG_IS_ANALOG,
		IS_INVERT /*! See \ref PIO_FLAG_IS_INVERT */ = PIO_FLAG_IS_INVERT,
		IS_FILTER /*! See \ref PIO_FLAG_IS_FILTER */ = PIO_FLAG_IS_FILTER,
		SET /*! Assigns the value of the port to one if the corresponding bit in the pin mask is set. */ = PIO_FLAG_SET,
		CLEAR /*! Assigns the value of the port to zero if the corresponding bit in the pin mask is set. */ = PIO_FLAG_CLEAR,
		ASSIGN /*! Assigns the value of the port to the value of the pin mask. */ = PIO_FLAG_ASSIGN
	};
};

API_OR_FLAGS_OPERATOR(PioFlags)


class PioAttributes : public PeriphAttributes<pio_attr_t>, public PioFlags {
public:
	PioAttributes(enum flags o_flags, u32 o_pinmask){
		set_flags(o_flags);
		set_pinmask(o_pinmask);
	}

	u32 o_flags() const { return m_attr.o_flags; }
	u32 o_pinmask() const { return m_attr.o_pinmask; }

	PioAttributes & set_flags(enum flags value){ m_attr.o_flags = value; return *this; }
	PioAttributes & set_pinmask(u32 value){ m_attr.o_pinmask = value; return *this; }

};

/*! \brief Pin Input/Output Class
 * \details This class controls pin input/output.
 *
 * \sa Hal::Pin
 *
 * Example:
 * \code
 *
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 *
 *	//This allocates a PIO object but does not affect HW state
 * 	Pio pio(2); //use pio port 2
 *
 * 	//This opens the port and sets the attributes -- same as pio.open(); pio.set_attr(1<<10, PIO_MODE_OUTPUT);
 * 	pio.init(Pio::FLAG_SET_OUTPUT, 1<<10);
 *
 * 	//Bits can be manipulated using clrmask() and setmask()
 * 	pio.clear_mask(1<<10); //clear bit 10 of port 2
 * 	pio.set_mask(1<<10); //set bit 10
 *
 *	//the pin can be reconfigured as an input
 * 	pio.set_attr(Pio::FLAG_SET_INPUT | Pio::FLAG_IS_PULLDOWN, (1<<10)); //set as an input with internal pulldown
 *
 * 	//get() is used to read the port
 * 	if( pio.get_value() & (1<<10) ){
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
class Pio : public Periph<pio_info_t, pio_attr_t, PioAttributes, 'p'>, public PioFlags {
public:
	Pio(port_t port);

	/*! \details Sets the specified pin mask */
	int set_mask(u32 mask) const;
	/*! \details Clears the specified mask. */
	int clear_mask(u32 mask) const;

	/*! \details Assigns the mask value to the port
	 *
	 * @param mask Bits that are set will be output as 1 and bits that are clear will be output as zero
	 * @return Zero on success
	 */
	int assign(u32 mask) const;

	/*! \details Gets the value of the port. */
	u32 get_value() const;

	/*! \details Sets the value of the port
	 *
	 * @param value The value to assign to the port
	 *
	 */
	int set_value(unsigned int value) const;

	using Device::operator <<;
	const Pio & operator << (u32 value) const {
		assign(value);
		return *this;
	}

private:

};

}

#endif /* SAPI_HAL_PIO_HPP_ */
