/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_I2C_HPP_
#define STFY_APP_I2C_HPP_

#include <iface/dev/i2c.h>
#include "Periph.hpp"



namespace hal {

/*! \brief I2C Peripheral Class
 * \details This class implements I2C device peripherals.
 *
 * A normal I2C transfer (I2C::SETUP_NORMAL_TRANSFER) updates the register
 * pointer and then either reads or writes data.  For example:
 * \code
 * #include <stfy/hal.hpp>
 *
 * u16 data;
 * I2C i2c(0); //use I2C port 0
 * i2c.init(100000); //100KHz
 *
 * i2c.setup(0x4C, I2C::SETUP_NORMAL_TRANSFER); //slave addr is 0x4C
 *
 * i2c.read(5, &data, sizeof(data)); //read 2 bytes from register 5
 * \endcode
 *
 * The code above will execute 2 I2C transactions. It will write 5 to the
 * I2C slave device register pointer. It will then read 2 bytes from the
 * slave device.
 *
 * If the slave device is not configured with a register pointer, you can
 * use the I2C::setup() method to perform other types of transactions.
 *
 * \code
 * i2c.setup(0x4C, SETUP_DATA_ONLY_TRANSFER);
 * i2c.write(&data, sizeof(data)); //this will write 2 bytes contained in data (location is ignored)
 * \endcode
 *
 */
class I2C : public Periph {
public:
	/*! \details Constructs an I2C object using the specified port. */
	I2C(port_t port);

	enum {
		SETUP_NORMAL_TRANSFER /*! Normal I2C transfer writes 8-bit pointer then reads or writes data */ = I2C_TRANSFER_NORMAL,
		SETUP_NORMAL_16_TRANSFER /*! Normal I2C transfer writes 16-bit pointer then reads or writes data */ = I2C_TRANSFER_NORMAL_16,
		SETUP_DATA_ONLY_TRANSFER /*! I2C transfer that does not write a pointer (just reads or writes data) */ = I2C_TRANSFER_DATA_ONLY,
		SETUP_WRITE_PTR_TRANSFER /*! I2C transfer that only writes the pointer (no data is transferred) */ = I2C_TRANSFER_WRITE_PTR
	};

	enum {
		MASTER /*! I2C acts as master */ = I2C_ATTR_FLAG_MASTER,
		SLAVE /*! I2C acts as slave */ = I2C_ATTR_FLAG_SLAVE,
		SLAVE_ACK_GENERAL_CALL /*! I2C slave will ack the general call */ = I2C_ATTR_FLAG_SLAVE_ACK_GENERAL_CALL,
		PULLUP /*! Enable internal resistor pullups where available */ = I2C_ATTR_FLAG_PULLUP
	};

	/*! \details Get the I2C attributes */
	int get_attr(i2c_attr_t & attr) const;
	/*! \details Set the I2C attributes */
	int set_attr(const i2c_attr_t & attr) const;
	/*! \details Setup an I2C transaction */
	int setup(const i2c_setup_t & setup) const;

	/*! \details Reset the I2C bus state */
	int reset() const;

	/*! \details Setup the bus to listen for transactions as a slave */
	int setup_slave(const i2c_slave_setup_t & setup) const;

	/*! \details Setup the bus to listen for transactions as a slave */
	int setup_slave(u8 addr, char * data, u16 size, u8 o_flags = 0){
		i2c_slave_setup_t setup;
		setup.addr = addr;
		setup.data = data;
		setup.size = size;
		setup.o_flags = o_flags;
		return setup_slave(setup);
	}

	/*! \details Get the last error */
	int get_err() const;
	int err() const { return get_err(); }


	/*! \details Sets up an I2C transaction using the slave addr and type */
	int setup(u16 slave_addr, u8 type = SETUP_NORMAL_TRANSFER) const {
		i2c_reqattr_t req;
		req.slave_addr = slave_addr;
		req.transfer = type;
		return setup(req);
	}

	/*! \details Sets the attributes using specified bitrate and pin assignment. */
	int set_attr(u32 bitrate = 100000, u8 pin_assign = 0, u16 o_flags = MASTER) const {
		i2c_attr_t attr;
		attr.bitrate = bitrate;
		attr.pin_assign = pin_assign;
		attr.o_flags = o_flags;
		return set_attr(attr);
	}

	/*! \details This method initializes the I2C port.  It opens the port
	 * and sets the attributes as specified.
	 * \code
	 * I2C i2c(0);  //Use port 0
	 * i2c.init(400000); //400KHz bitrate
	 * \endcode
	 *
	 */
	int init(int bitrate = 100000, int pin_assign = 0, u16 o_flags = MASTER){
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

	/*! \details Sets (or clears) a specific bit in a a register
	 * on an I2C device.
	 *
	 * @param loc The register offset value
	 * @param bit The bit to set (or clear)
	 * @param high true to set the bit and false to clear it
	 * @return Zero on success
	 */
	int set(int loc, int bit, bool high = true);

	/*! \details Clears the bit in a register on an I2C device. */
	int clear(int loc, int bit){ return set(loc, bit, false); }

private:

};

};


#endif /* STFY_APP_I2C_HPP_ */
