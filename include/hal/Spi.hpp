/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SPI_HPP_
#define SAPI_SPI_HPP_

#include <sos/dev/spi.h>
#include "Periph.hpp"


namespace hal {


/*! \brief SPI Class
 * \details This class gives access to a SPI port.
 */
class Spi : public Periph<spi_info_t, spi_attr_t, 's'>{
public:
	Spi(port_t port);

	enum {
		FLAG_FORMAT_SPI/*! SPI Format */ = SPI_FLAG_FORMAT_SPI,
		FLAG_FORMAT_TI /*! TI Format */ = SPI_FLAG_FORMAT_TI,
		FLAG_FORMAT_MICROWIRE /*! Microwire format */ = SPI_FLAG_FORMAT_MICROWIRE,
		FLAG_MODE0 /*! SPI Mode 0 */ = SPI_FLAG_MODE0,
		FLAG_MODE1 /*! SPI Mode 1 */ = SPI_FLAG_MODE1,
		FLAG_MODE2 /*! SPI Mode 2 */ = SPI_FLAG_MODE2,
		FLAG_MODE3 /*! SPI Mode 3 */ = SPI_FLAG_MODE3,
		FLAG_SET_MASTER /*! SPI Master */ = SPI_FLAG_SET_MASTER,
		FLAG_SET_SLAVE /*! SPI Slave */ = SPI_FLAG_SET_SLAVE,
		FLAG_FULL_DUPLEX /*! Full duplex mode (data is written first then read into the same buffer) */ = SPI_FLAG_FULL_DUPLEX,
		FLAG_HALF_DUPLEX /*! Half duplex mode (default mode) */ = SPI_FLAG_HALF_DUPLEX
	};

	/*! \details swap a byte on the SPI bus */
	int swap(int byte);

	/*! \details Set SPI attributes using values specified */
	int set_attr(u32 o_flags, mcu_pin_t miso, mcu_pin_t mosi, mcu_pin_t sck, mcu_pin_t cs, u32 freq, u32 width = 8){
		spi_attr_t attr;
		attr.o_flags = o_flags;
		attr.freq = freq;
		attr.pin_assignment[0] = miso;
		attr.pin_assignment[1] = mosi;
		attr.pin_assignment[2] = sck;
		attr.pin_assignment[3] = cs;
		attr.width = width;
		return Periph::set_attr(attr);
	}

	/*! \details initialize the SPI port */
	int init(u32 o_flags, mcu_pin_t miso, mcu_pin_t mosi, mcu_pin_t sck, mcu_pin_t cs, u32 freq, u32 width = 8){

		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, miso, mosi, sck, cs, freq, width);
	}



private:

};

};

#endif /* SAPI_SPI_HPP_ */
