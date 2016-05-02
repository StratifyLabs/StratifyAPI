/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_SPI_HPP_
#define STFY_APP_SPI_HPP_

#include <iface/dev/spi.h>
#include "Periph.hpp"


namespace Hal {


/*! \brief SPI Class
 * \details This class gives access to a SPI port.
 */
class Spi : public Periph {
public:
	Spi(port_t port);
	/*! \brief get the SPI attributes */
	int attr(spi_attr_t * attr);
	/*! \brief set the SPI attributes */
	int setattr(const spi_attr_t * attr);
	/*! \brief swap a byte on the SPI bus */
	int swap(int byte);
	/*! \brief set the pointer for duplex operations */
	int setduplex(void * buf);

	enum {
		MODE0 = SPI_ATTR_MODE0,
		MODE1 = SPI_ATTR_MODE1,
		MODE2 = SPI_ATTR_MODE2,
		MODE3 = SPI_ATTR_MODE3,
		MASTER = SPI_ATTR_MASTER,
		SLAVE = SPI_ATTR_SLAVE,
		FORMAT_SPI = SPI_ATTR_FORMAT_SPI,
		FORMAT_TI = SPI_ATTR_FORMAT_TI,
		FORMAT_MICROWIRE = SPI_ATTR_FORMAT_MICROWIRE
	};

#ifdef __MCU_ONLY__
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int read(Aio & aio);
	int write(Aio & aio);
	int close(void);
#endif

	/*! \brief Set SPI attributes using values specified */
	int setattr(int bitrate = 2000000,
			int pin_assign = 0,
			uint8_t mode = SPI_ATTR_MODE0,
			uint8_t master = SPI_ATTR_MASTER,
			uint8_t format = SPI_ATTR_FORMAT_SPI,
			uint8_t width = 8){
		spi_attr_t attr;
		attr.bitrate = bitrate;
		attr.mode = mode;
		attr.pin_assign = pin_assign;
		attr.format = format;
		attr.width = width;
		attr.master = master;
		return setattr(&attr);
	}

	/*! \brief initialize the SPI port */
	int init(int bitrate = 2000000,
			int pin_assign = 0,
			uint8_t mode = SPI_ATTR_MODE0,
			uint8_t master = SPI_ATTR_MASTER,
			uint8_t format = SPI_ATTR_FORMAT_SPI,
			uint8_t width = 8){
		if( open() < 0 ){
			return -1;
		}
		return setattr(bitrate, pin_assign, mode, master, format, width);
	}



private:

};

};

#endif /* STFY_APP_SPI_HPP_ */
