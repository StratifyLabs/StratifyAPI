/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_I2C_HPP_
#define STFY_APP_I2C_HPP_

#include <iface/dev/i2c.h>
#include "Periph.hpp"



namespace hal {

/*! \brief I2C Peripheral Class
 * \details This class implements I2C device peripherals.
 */
class I2C : public Periph {
public:
	/*! \details Initialize the instance with \a port */
	I2C(port_t port);

	enum {
		SETUP_NORMAL_TRANSFER /*! \brief Normal I2C transfer writes 8-bit pointer then reads or writes data */ = I2C_TRANSFER_NORMAL,
		SETUP_NORMAL_16_TRANSFER /*! \brief Normal I2C transfer writes 16-bit pointer then reads or writes data */ = I2C_TRANSFER_NORMAL_16,
		SETUP_DATA_ONLY_TRANSFER /*! \brief I2C transfer that does not write a pointer (just reads or writes data) */ = I2C_TRANSFER_DATA_ONLY,
		SETUP_WRITE_PTR_TRANSFER /*! \brief I2C transfer that only writes the pointer (no data is transferred) */ = I2C_TRANSFER_WRITE_PTR
	};

	/*! \details Get the I2C attributes */
	int get_attr(i2c_attr_t & attr);
	/*! \details Set the I2C attributes */
	int set_attr(const i2c_attr_t & attr);
	/*! \details Setup an I2C transaction */
	int setup(const i2c_reqattr_t & req);

	/*! \details Get the last error */
	int err();

#ifdef __MCU_ONLY__
	using Pblock::read;
	using Pblock::write;
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int close();
#endif


	/*! \details Setup an I2C transaction using the slave addr and type */
	int setup(u16 slave_addr, u8 type = SETUP_NORMAL_TRANSFER){
		i2c_reqattr_t req;
		req.slave_addr = slave_addr;
		req.transfer = type;
		return setup(req);
	}

	/*! \details Set attributes using specified bitrate and pin assignment. */
	int set_attr(u32 bitrate = 100000, u8 pin_assign = 0){
		i2c_attr_t attr;
		attr.bitrate = bitrate;
		attr.pin_assign = pin_assign;
		return set_attr(attr);
	}

	/*! \details This method initializes the I2C port.  It opens the port
	 * and sets the attributes as specified.
	 * \code
	 * I2c i2c(0);  //Use port 0
	 * i2c.init(400000); //400KHz bitrate
	 * \endcode
	 *
	 */
	int init(int bitrate = 100000, int pin_assign = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(bitrate, pin_assign);
	}


	using Periph::read;
	using Periph::write;

	/*! \details Read the value of a register on an I2C device */
	int read(int loc, u8 & reg);
	/*! \details Write the value of a register on an I2C device */
	int write(int loc, u8 reg);

	/*! \details This sets (or clears) a specific bit in a a register
	 * on an I2C device
	 * @param loc The register offset value
	 * @param bit The bit to set (or clear)
	 * @param high true to set the bit and false to clear it
	 * @return Zero on success
	 */
	int set(int loc, int bit, bool high = true);

	/*! \details Clear the bit in a register on an I2C device */
	inline int clear(int loc, int bit){ return set(loc, bit, false); }

private:

};

};


#endif /* STFY_APP_I2C_HPP_ */
