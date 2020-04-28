/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_HAL_PIO_HPP_
#define SAPI_HAL_PIO_HPP_

#include <sos/dev/pio.h>
#include "Periph.hpp"

namespace hal {

class PioFlags {
public:
	enum flags {
		flag_set_input /*! Sets the pinmask as input */ = PIO_FLAG_SET_INPUT,
		flag_set_output /*! Sets the pinmask as output */ = PIO_FLAG_SET_OUTPUT,
		flag_is_pullup /*! When setting as input, enables the pullup */ = PIO_FLAG_IS_PULLUP,
		flag_is_pulldown /*! When setting as input, enables the pulldown */ = PIO_FLAG_IS_PULLDOWN,
		flag_is_repeater /*! When setting as input, enables repeater mode (if available) */ = PIO_FLAG_IS_REPEATER,
		flag_is_float /*! When setting as input, enables floating mode (no pullup or pulldown) */ = PIO_FLAG_IS_FLOAT,
		flag_is_speed_low /*! When setting as output, enables low speed mode */ = PIO_FLAG_IS_SPEED_LOW,
		flag_is_speed_medium /*! When setting as output, enables medium speed mode */ = PIO_FLAG_IS_SPEED_MEDIUM,
		flag_is_speed_high /*! When setting as output, enables high speed mode */ = PIO_FLAG_IS_SPEED_HIGH,
		flag_is_speed_blazing /*! When setting as output, enables blazing speed mode */ = PIO_FLAG_IS_SPEED_BLAZING,
		flag_is_opendrain /*! When setting as output, enables open drain mode */ = PIO_FLAG_IS_OPENDRAIN,
		flag_is_hysteresis /*! When setting as input, enables hysteresis */ = PIO_FLAG_IS_HYSTERESIS,
		flag_is_direction_only /*! When set, only affects the direction withouth setting other attributes */ = PIO_FLAG_IS_DIRONLY,
		flag_is_analog /*! When setting as input, sets pin as analog mode */ = PIO_FLAG_IS_ANALOG,
		flag_is_invert /*! See \ref PIO_FLAG_IS_INVERT */ = PIO_FLAG_IS_INVERT,
		flag_is_filter /*! See \ref PIO_FLAG_IS_FILTER */ = PIO_FLAG_IS_FILTER,
		flag_set /*! Assigns the value of the port to one if the corresponding bit in the pin mask is set. */ = PIO_FLAG_SET,
		flag_clear /*! Assigns the value of the port to zero if the corresponding bit in the pin mask is set. */ = PIO_FLAG_CLEAR,
		flag_assign /*! Assigns the value of the port to the value of the pin mask. */ = PIO_FLAG_ASSIGN
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
