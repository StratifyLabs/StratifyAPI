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
	/*! \details get the SPI attributes */
	int attr(spi_attr_t * attr);
	/*! \details set the SPI attributes */
	int setattr(const spi_attr_t * attr);
	/*! \details swap a byte on the SPI bus */
	int swap(int byte);
	/*! \details This method sets the duplex buffer for full duplex operations.
	 * If data is written to the SPI using write() and the duplex buffer is not null, data will
	 * simultaneously be read from the SPI bus into the duplex buffer.
	 *
	 * Conversely, when data is read() from the SPI bus, the duplex buffer will be written to
	 * the SPI bus if the buffer is non-zero.
	 *
	 * Example
	 *
	 * \code
	 *
	 * char duplex_buffer[64];
	 * char buffer[64];
	 * Spi spi(0);
	 *
	 * spi.init(1000000);
	 *
	 * spi.setduplex(duplex_buffer);
	 *
	 * spi.write(buffer, 64); //buffer is written to SPI Bus and data read from the SPI is written to duplex buffer
	 * spi.read(buffer, 64); //buffer is read from the SPI bus and data in duplex buffer is written to the bus
	 *
	 * spi.close();
	 * \endcode
	 *
	 * If the duplex buffer is null, the SPI MOSI pin is high when reading data.
	 *
	 *
	 */
	int setduplex(void * buf);

	/*! \details SPI attribute options */
	enum {
		MODE0 /*! SPI mode 0 */ = SPI_ATTR_MODE0,
		MODE1 /*! SPI mode 1 */ = SPI_ATTR_MODE1,
		MODE2 /*! SPI mode 2 */ = SPI_ATTR_MODE2,
		MODE3 /*! SPI mode 3 */ = SPI_ATTR_MODE3,
		MASTER /*! SPI Master */ = SPI_ATTR_MASTER,
		SLAVE /*! SPI Slave */ = SPI_ATTR_SLAVE,
		FORMAT_SPI /*! SPI Format */ = SPI_ATTR_FORMAT_SPI,
		FORMAT_TI /*! TI format */ = SPI_ATTR_FORMAT_TI,
		FORMAT_MICROWIRE /*! Microwire format */ = SPI_ATTR_FORMAT_MICROWIRE
	};

#ifdef __MCU_ONLY__
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int read(Aio & aio);
	int write(Aio & aio);
	int close(void);
#endif

	/*! \details Set SPI attributes using values specified */
	int setattr(int bitrate = 2000000,
			int pin_assign = 0,
			u8 mode = MODE0,
			u8 master = MASTER,
			u8 format = FORMAT_SPI,
			u8 width = 8){
		spi_attr_t attr;
		attr.bitrate = bitrate;
		attr.mode = mode;
		attr.pin_assign = pin_assign;
		attr.format = format;
		attr.width = width;
		attr.master = master;
		return setattr(&attr);
	}

	/*! \details initialize the SPI port */
	int init(int bitrate = 2000000,
			int pin_assign = 0,
			u8 mode = SPI_ATTR_MODE0,
			u8 master = SPI_ATTR_MASTER,
			u8 format = SPI_ATTR_FORMAT_SPI,
			u8 width = 8){
		if( open() < 0 ){
			return -1;
		}
		return setattr(bitrate, pin_assign, mode, master, format, width);
	}



private:

};

};

#endif /* STFY_APP_SPI_HPP_ */
