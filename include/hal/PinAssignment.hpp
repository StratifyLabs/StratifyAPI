/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef HAL_PINASSIGNMENT_HPP_
#define HAL_PINASSIGNMENT_HPP_

#include "../var/Item.hpp"

namespace hal {

/*! \brief Pin Assignment Class
 * \details The Pin Assignment class is a template
 * class for manipulating pin assignment data structures.
 *
 * The following is an example of how to configure the SPI using
 * the hal::SpiPinAssignment class (which inherits this class).
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * SpiPinAssignment pin_assignment;
 * pin_assignment->mosi = mcu_pin(2,0);
 * pin_assignment->sck = mcu_pin(2,0);
 * pin_assignment->miso = mcu_pin(2,0);
 *
 * Spi spi(0);
 *
 * spi.init(
 *  Spi::FLAG_SET_MASTER|Spi::FLAG_IS_MODE0|Spi::FLAG_IS_FORMAT_SPI,
 *  10000000, 8, pin_assignment
 * );
 *
 * \endcode
 *
 *
 */
template<typename pin_assignment_type> class PinAssignment {
public:
	PinAssignment(){ clear(); }

	/*! \details Returns a const pointer to the pin assignment data structure.
	 *
	 * This operator makes it easy to pass pin assignment data to set_attr()
	 * methods.
	 *
	 */
	operator const pin_assignment_type * () const { return &m_pin_assignment; }

	/*! \details Provides read and write access to the members of
	 * the pin assignment structure.
	 */
	pin_assignment_type * operator ->(){ return &m_pin_assignment; }

	/*! \details Clears the pin assignment data structure (assigns 0xff
	 * to all ports/pins.
	 */
	void clear(){
		memset(&m_pin_assignment,
				0xff,
				sizeof(m_pin_assignment));
	}

	/*! \details Returns the number of bytes in the data structure. */
	u32 size() const { return sizeof(pin_assignment_type); }

	/*! \detais Returns the number of pins in the pin assignment data structure. */
	u32 count() const { return size() / sizeof(mcu_pin_t); }


private:
	pin_assignment_type m_pin_assignment;
};

} /* namespace hal */

#endif /* HAL_PINASSIGNMENT_HPP_ */
