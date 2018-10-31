/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_PIO_HPP_
#define SAPI_PIO_HPP_

#include <sos/dev/pio.h>
#include "Periph.hpp"

namespace hal {


class PioAttributes : public PeriphAttributes<pio_attr_t> {
public:
	PioAttributes(u32 o_flags, u32 o_pinmask){
		set_flags(o_flags);
		set_pinmask(o_pinmask);
	}

	u32 o_flags() const { return m_attr.o_flags; }
	u32 o_pinmask() const { return m_attr.o_pinmask; }

	void set_flags(u32 value){ m_attr.o_flags = value; }
	void set_pinmask(u32 value){ m_attr.o_pinmask = value; }

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
class Pio : public Periph<pio_info_t, pio_attr_t, PioAttributes, 'p'> {
public:
	Pio(port_t port);

	enum {
		FLAG_SET_INPUT = PIO_FLAG_SET_INPUT,
		FLAG_SET_OUTPUT = PIO_FLAG_SET_OUTPUT,
		FLAG_IS_PULLUP = PIO_FLAG_IS_PULLUP,
		FLAG_IS_PULLDOWN = PIO_FLAG_IS_PULLDOWN,
		FLAG_IS_REPEATER = PIO_FLAG_IS_REPEATER,
		FLAG_IS_FLOAT = PIO_FLAG_IS_FLOAT,
		FLAG_IS_SPEED_LOW = PIO_FLAG_IS_SPEED_LOW,
		FLAG_IS_SPEED_MEDIUM = PIO_FLAG_IS_SPEED_MEDIUM,
		FLAG_IS_SPEED_HIGH = PIO_FLAG_IS_SPEED_HIGH,
		FLAG_IS_SPEED_BLAZING = PIO_FLAG_IS_SPEED_BLAZING,
		FLAG_IS_OPENDRAIN = PIO_FLAG_IS_OPENDRAIN,
		FLAG_IS_HYSTERESIS = PIO_FLAG_IS_HYSTERESIS,
		FLAG_IS_DIRONLY = PIO_FLAG_IS_DIRONLY,
		FLAG_IS_ANALOG = PIO_FLAG_IS_ANALOG,
		FLAG_IS_INVERT = PIO_FLAG_IS_INVERT,
		FLAG_IS_FILTER = PIO_FLAG_IS_FILTER,
		FLAG_SET = PIO_FLAG_SET,
		FLAG_CLEAR = PIO_FLAG_CLEAR,
		FLAG_ASSIGN = PIO_FLAG_ASSIGN,

		SET_INPUT /*! See \ref PIO_FLAG_SET_INPUT */ = PIO_FLAG_SET_INPUT,
		SET_OUTPUT /*! See \ref PIO_FLAG_SET_OUTPUT */ = PIO_FLAG_SET_OUTPUT,
		IS_PULLUP /*! See \ref PIO_FLAG_IS_PULLUP */ = PIO_FLAG_IS_PULLUP,
		IS_PULLDOWN /*! See \ref PIO_FLAG_IS_PULLDOWN */ = PIO_FLAG_IS_PULLDOWN,
		IS_REPEATER /*! See \ref PIO_FLAG_IS_REPEATER */ = PIO_FLAG_IS_REPEATER,
		IS_FLOAT /*! See \ref PIO_FLAG_IS_FLOAT */ = PIO_FLAG_IS_FLOAT,
		IS_SPEED_LOW /*! See \ref PIO_FLAG_IS_SPEED_LOW */ = PIO_FLAG_IS_SPEED_LOW,
		IS_SPEED_MEDIUM /*! See \ref PIO_FLAG_IS_SPEED_MEDIUM */ = PIO_FLAG_IS_SPEED_MEDIUM,
		IS_SPEED_HIGH /*! See \ref PIO_FLAG_IS_SPEED_HIGH */ = PIO_FLAG_IS_SPEED_HIGH,
		IS_SPEED_BLAZING /*! See \ref PIO_FLAG_IS_SPEED_BLAZING */ = PIO_FLAG_IS_SPEED_BLAZING,
		IS_OPENDRAIN /*! See \ref PIO_FLAG_IS_OPENDRAIN */ = PIO_FLAG_IS_OPENDRAIN,
		IS_HYSTERESIS /*! See \ref PIO_FLAG_IS_HYSTERESIS */ = PIO_FLAG_IS_HYSTERESIS,
		IS_DIRONLY /*! See \ref PIO_FLAG_IS_DIRONLY */ = PIO_FLAG_IS_DIRONLY,
		IS_ANALOG /*! See \ref PIO_FLAG_IS_ANALOG */ = PIO_FLAG_IS_ANALOG,
		IS_INVERT /*! See \ref PIO_FLAG_IS_INVERT */ = PIO_FLAG_IS_INVERT,
		IS_FILTER /*! See \ref PIO_FLAG_IS_FILTER */ = PIO_FLAG_IS_FILTER,
		SET /*! See \ref PIO_FLAG_SET */ = PIO_FLAG_SET,
		CLEAR /*! See \ref PIO_FLAG_CLEAR */ = PIO_FLAG_CLEAR,
		ASSIGN /*! See \ref PIO_FLAG_ASSIGN */ = PIO_FLAG_ASSIGN
	};

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

#endif /* SAPI_PIO_HPP_ */
