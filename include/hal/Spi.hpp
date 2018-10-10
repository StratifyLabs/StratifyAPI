/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SPI_HPP_
#define SAPI_SPI_HPP_

#include <sos/dev/spi.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"


namespace hal {

/*! \brief SPI Pin Assignment
 * \details This class allows simple manipulation of the spi_pin_assignment_t.
 *
 */
class SpiPinAssignment : public PinAssignment<spi_pin_assignment_t>{};


class SpiAttributes : public PinAssignmentPeriphAttr<spi_attr_t, spi_pin_assignment_t> {
public:

	SpiAttributes(u32 o_flags = SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_HALF_DUPLEX,
				  u32 freq = 1000000,
				  u8 width = 8){
		set_freq(freq);
		set_width(width);
		set_flags(o_flags);
	}

	void set_miso(const mcu_pin_t & pin){ m_attr.pin_assignment.miso = pin; }
	void set_mosi(const mcu_pin_t & pin){ m_attr.pin_assignment.mosi = pin; }
	void set_sck(const mcu_pin_t & pin){ m_attr.pin_assignment.sck = pin; }
	void set_cs(const mcu_pin_t & pin){ m_attr.pin_assignment.cs = pin; }
	void set_width(u8 value){ m_attr.width = value; }

	mcu_pin_t miso() const { return m_attr.pin_assignment.miso; }
	mcu_pin_t mosi() const { return m_attr.pin_assignment.mosi; }
	mcu_pin_t sck() const { return m_attr.pin_assignment.sck; }
	mcu_pin_t cs() const { return m_attr.pin_assignment.cs; }
	u8 width() const { return m_attr.width; }

private:


};

typedef SpiAttributes SpiAttr;


/*! \brief SPI Class
 * \details This class gives access to a SPI port.
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 *   char buffer[16];
 *   Spi spi(0);           //access to SPI port 0
 *   spi.init();           //init SPI with default settings from BSP
 *   spi.read(buffer, 16); //read 16 bytes from the SPI
 *   spi.close();          //close the SPI (power it off)
 *   return 0;
 * }
 * \endcode
 */
class Spi : public Periph<spi_info_t, spi_attr_t, 's'>{
public:

	/*! \details Constructs a SPI object using \a port. */
	Spi(port_t port);

	enum {
		FLAG_IS_FORMAT_SPI = SPI_FLAG_IS_FORMAT_SPI,
		FLAG_IS_FORMAT_TI = SPI_FLAG_IS_FORMAT_TI,
		FLAG_IS_FORMAT_MICROWIRE = SPI_FLAG_IS_FORMAT_MICROWIRE,
		FLAG_IS_MODE0 = SPI_FLAG_IS_MODE0,
		FLAG_IS_MODE1 = SPI_FLAG_IS_MODE1,
		FLAG_IS_MODE2 = SPI_FLAG_IS_MODE2,
		FLAG_IS_MODE3 = SPI_FLAG_IS_MODE3,
		FLAG_SET_MASTER = SPI_FLAG_SET_MASTER,
		FLAG_SET_SLAVE = SPI_FLAG_SET_SLAVE,
		FLAG_IS_FULL_DUPLEX = SPI_FLAG_IS_FULL_DUPLEX,
		FLAG_IS_HALF_DUPLEX = SPI_FLAG_IS_HALF_DUPLEX,

		IS_FORMAT_SPI/*! See \ref SPI_FLAG_IS_FORMAT_SPI */ = SPI_FLAG_IS_FORMAT_SPI,
		IS_FORMAT_TI /*! See \ref SPI_FLAG_IS_FORMAT_TI */ = SPI_FLAG_IS_FORMAT_TI,
		IS_FORMAT_MICROWIRE /*! See \ref SPI_FLAG_IS_FORMAT_MICROWIRE */ = SPI_FLAG_IS_FORMAT_MICROWIRE,
		IS_MODE0 /*! See \ref SPI_FLAG_IS_MODE0 */ = SPI_FLAG_IS_MODE0,
		IS_MODE1 /*! See \ref SPI_FLAG_IS_MODE1 */ = SPI_FLAG_IS_MODE1,
		IS_MODE2 /*! See \ref SPI_FLAG_IS_MODE2 */ = SPI_FLAG_IS_MODE2,
		IS_MODE3 /*! See \ref SPI_FLAG_IS_MODE3 */ = SPI_FLAG_IS_MODE3,
		SET_MASTER /*! See \ref SPI_FLAG_SET_MASTER */ = SPI_FLAG_SET_MASTER,
		SET_SLAVE /*! See \ref SPI_FLAG_SET_SLAVE */ = SPI_FLAG_SET_SLAVE,
		IS_FULL_DUPLEX /*! See \ref SPI_FLAG_IS_FULL_DUPLEX */ = SPI_FLAG_IS_FULL_DUPLEX,
		IS_HALF_DUPLEX /*! See \ref SPI_FLAG_IS_HALF_DUPLEX */ = SPI_FLAG_IS_HALF_DUPLEX,
	};

	/*! \details swap a byte on the SPI bus */
	int swap(int byte) const;

	int set_attr(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0) const {
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

	int initialize(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, width, pin_assignment);
	}
	int init(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0){ return initialize(o_flags, freq, width, pin_assignment); }

	using Periph::init;
	using Periph::set_attr;
	using Periph::initialize;
	using Periph::set_attributes;

#if !defined __link
	int transfer(const void * write_data, void * read_data, int nbytes);
#endif

private:

};

}

#endif /* SAPI_SPI_HPP_ */
