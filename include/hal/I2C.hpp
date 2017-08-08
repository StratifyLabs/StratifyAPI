/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_I2C_HPP_
#define SAPI_I2C_HPP_

#include <sos/dev/i2c.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"



namespace hal {

/*! \brief I2C Pin Assignment
 * \details This class allows simple manipulation of the i2c_pin_assignment_t.
 */
class I2CPinAssignment : public PinAssignment<i2c_pin_assignment_t>{};

/*! \brief I2C Peripheral Class
 * \details This class implements I2C device peripherals.
 *
 * An I2C ptr/data transfer (I2C::FLAG_PREPARE_PTR_DATA) updates the register
 * pointer and then either reads or writes data.  For example:
 * \code
 * #include <sapi/hal.hpp>
 *
 * u16 data;
 * I2C i2c(0); //use I2C port 0
 * i2c.init(I2C::FLAG_SET_MASTER); //Init as a master using default pin assignment
 *
 * i2c.prepare(0x4C); //slave addr is 0x4C - operation is 8-bit ptr plus data
 * i2c.read(5, &data, sizeof(data)); //read 2 bytes from register 5
 * i2c.close();
 * \endcode
 *
 * The code above will execute 2 I2C transactions. It will write 5 to the
 * I2C slave device register pointer. It will then read 2 bytes from the
 * slave device.
 *
 *
 */
class I2C : public Periph<i2c_info_t, i2c_attr_t, 'i'> {
public:
	/*! \details Constructs an I2C object using the specified port. */
	I2C(port_t port);

	enum {
		ERROR_NONE /*! No errors */ = I2C_ERROR_NONE,
		ERROR_START /*! Error while starting */ = I2C_ERROR_START,
		ERROR_WRITE /*! Error while writing */ = I2C_ERROR_WRITE,
		ERROR_ACK /*! Ack Error (most common error for a mis-wired hardware) */ = I2C_ERROR_ACK,
		ERROR_STOP /*! Error while stopping */ = I2C_ERROR_STOP,
		ERROR_MASTER_ACK /*! The master could not create an ACK */ = I2C_ERROR_MASTER_ACK,
		ERROR_BUS_BUSY /*! The Bus is busy (happens with multi-masters on bus) */ = I2C_ERROR_BUS_BUSY,
		ERROR_LONG_SLEW = I2C_ERROR_LONG_SLEW,
		ERROR_ARBITRATION_LOST /*! Arbitration lost on multi-master bus */ = I2C_ERROR_ARBITRATION_LOST
	};

	enum {
		FLAG_NONE = I2C_FLAG_NONE,
		FLAG_SET_MASTER /*! Operate as a master I2C bus */ = I2C_FLAG_SET_MASTER,
		FLAG_SET_SLAVE/*! Operate as a slave (ignored if master is set) */ = I2C_FLAG_SET_SLAVE,
		FLAG_IS_SLAVE_ACK_GENERAL_CALL /*! If slave operation, ack general call */ = I2C_FLAG_IS_SLAVE_ACK_GENERAL_CALL,
		FLAG_IS_PULLUP /*! Enable internal pullups if available (ignore otherwise) */ = I2C_FLAG_IS_PULLUP,
		FLAG_PREPARE_PTR_DATA /*! This prepares the driver to write the ptr then read/write data */ = I2C_FLAG_PREPARE_PTR_DATA,
		FLAG_IS_PTR_16 /*! This prepares the driver to write a 16-bit ptr then read/write data */ = I2C_FLAG_IS_PTR_16,
		FLAG_PREPARE_PTR /*! This will write the ptr value only without writing or reading any data. */ = I2C_FLAG_PREPARE_PTR,
		FLAG_PREPARE_DATA /*! This will read/write data without first writing the pointer information */ = I2C_FLAG_PREPARE_DATA,
		FLAG_IS_SLAVE_ADDR0 /*! If hardware supports multiple slave addrs, use the first slot (default) */ = I2C_FLAG_IS_SLAVE_ADDR0,
		FLAG_IS_SLAVE_ADDR1 /*! If hardware supports multiple slave addrs, use the second slot */ = I2C_FLAG_IS_SLAVE_ADDR1,
		FLAG_IS_SLAVE_ADDR2 /*! If hardware supports multiple slave addrs, use the third slot */ = I2C_FLAG_IS_SLAVE_ADDR2,
		FLAG_IS_SLAVE_ADDR3 /*! If hardware supports multiple slave addrs, use the fourth slot */ = I2C_FLAG_IS_SLAVE_ADDR3,
		FLAG_IS_SLAVE_PTR_8 /*! Use a 8-bit address pointer when accessing data (default) */ = I2C_FLAG_IS_SLAVE_PTR_8,
		FLAG_IS_SLAVE_PTR_16 /*! Use a 16-bit address pointer when accessing data (set automatically is size > 255) */ = I2C_FLAG_IS_SLAVE_PTR_16,
		FLAG_RESET /*! Reset the state of the I2C */ = I2C_FLAG_RESET,
		FLAG_STRETCH_CLOCK = I2C_FLAG_STRETCH_CLOCK,
		FLAG_IS_NO_STOP /*! Don't issue a stop condition when complete (use with I2C_FLAG_PREPARE_DATA) */ = I2C_FLAG_IS_NO_STOP
	};


	/*! \details Prepares an I2C transaction.
	 *
	 * @param slave_addr The slave address for the transaction
	 * @param o_flags The flags for the transaction
	 * @return Zero on success
	 *
	 * The \a o_flags can be:
	 * - FLAG_PREPARE_PTR_DATA (default)
	 * - FLAG_PREPARE_PTR_16_DATA
	 * - FLAG_PREPARE_PTR
	 * - FLAG_PREPARE_DATA
	 *
	 */
	int prepare(u8 slave_addr, u32 o_flags = FLAG_PREPARE_PTR_DATA) const;

	/*! \details Reset the I2C bus state */
	int reset() const;

	/*! \details Get the last error */
	int get_err() const;

	/*! \details Sets the attributes of the I2C bus.
	 *
	 * @param o_flags Flag bitmask
	 * @param freq Bitrate
	 * @param pin_assignment Pin assignment or null to use default pin assignment
	 * @return Zero on success
	 */
	int set_attr(u32 o_flags, u32 freq, const i2c_pin_assignment_t * pin_assignment = 0){
		i2c_attr_t attr;
		attr.o_flags = o_flags;
		attr.freq = freq;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(i2c_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(i2c_pin_assignment_t));
		}
		return set_attr(attr);
	}

	/*! \details This method initializes the I2C port.
	 *
	 * @param o_flags Flag bitmask
	 * @param freq Bitrate
	 * @param pin_assignment Pin assignment or null to use default pin assignment
	 * @return Zero on success
	 *
	 * This method calls open() then set_attr().
	 *
	 */
	int init(u32 o_flags, u32 freq, const i2c_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, pin_assignment);
	}


	using Periph::read;
	using Periph::write;
	using Periph::init;
	using Periph::set_attr;

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


#endif /* SAPI_I2C_HPP_ */
