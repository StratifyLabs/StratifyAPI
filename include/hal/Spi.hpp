/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SPI_HPP_
#define SAPI_SPI_HPP_

#include <sos/dev/spi.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"


namespace hal {

/*! \brief SPI Pin Assignment
 * \details This class allows simple manipulation of the spi_pin_assignment_t.
 */
class SpiPinAssignment : public PinAssignment<spi_pin_assignment_t>{};


/*! \brief SPI Class
 * \details This class gives access to a SPI port.
 */
class Spi : public Periph<spi_info_t, spi_attr_t, 's'>{
public:
	Spi(port_t port);

	enum {
		FLAG_IS_FORMAT_SPI/*! SPI Format */ = SPI_FLAG_IS_FORMAT_SPI,
		FLAG_IS_FORMAT_TI /*! TI Format */ = SPI_FLAG_IS_FORMAT_TI,
		FLAG_IS_FORMAT_MICROWIRE /*! Microwire format */ = SPI_FLAG_IS_FORMAT_MICROWIRE,
		FLAG_IS_MODE0 /*! SPI Mode 0 */ = SPI_FLAG_IS_MODE0,
		FLAG_IS_MODE1 /*! SPI Mode 1 */ = SPI_FLAG_IS_MODE1,
		FLAG_IS_MODE2 /*! SPI Mode 2 */ = SPI_FLAG_IS_MODE2,
		FLAG_IS_MODE3 /*! SPI Mode 3 */ = SPI_FLAG_IS_MODE3,
		FLAG_SET_MASTER /*! SPI Master */ = SPI_FLAG_SET_MASTER,
		FLAG_SET_SLAVE /*! SPI Slave */ = SPI_FLAG_SET_SLAVE,
		FLAG_IS_FULL_DUPLEX /*! Full duplex mode (data is written first then read into the same buffer) */ = SPI_FLAG_IS_FULL_DUPLEX,
		FLAG_IS_HALF_DUPLEX /*! Half duplex mode (default mode) */ = SPI_FLAG_IS_HALF_DUPLEX,
	};

	/*! \details swap a byte on the SPI bus */
	int swap(int byte);

	/*! \details Set SPI attributes using values specified */
	int set_attr(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0){
		spi_attr_t attr;
		attr.o_flags = o_flags;
		attr.freq = freq;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(spi_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(spi_pin_assignment_t));
		}
		attr.width = width;
		return Periph::set_attr(attr);
	}

	/*! \details initialize the SPI port */
	int init(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0){

		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, width, pin_assignment);
	}

	using Periph::init;
	using Periph::set_attr;

private:

};

};

#endif /* SAPI_SPI_HPP_ */
