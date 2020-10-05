/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_HAL_I2C_HPP_
#define SAPI_HAL_I2C_HPP_

#include <sos/dev/i2c.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"



namespace hal {

class I2CFlags {
public:
	enum flags {
		flag_set_master /*! Operate as a master I2C bus */ = I2C_FLAG_SET_MASTER,
		flag_set_slave /*! Operate as a slave (ignored if master is set) */ = I2C_FLAG_SET_SLAVE,
		flag_is_slave_ack_general_call /*! If slave operation, ack general call */ = I2C_FLAG_IS_SLAVE_ACK_GENERAL_CALL,
		flag_is_pullup /*! Enable internal pullups if available (ignore otherwise) */ = I2C_FLAG_IS_PULLUP,
		flag_prepare_ptr_data /*! This prepares the driver to write the ptr then read/write data */ = I2C_FLAG_PREPARE_PTR_DATA,
		flag_is_ptr_16 /*! This prepares the driver to write a 16-bit ptr then read/write data */ = I2C_FLAG_IS_PTR_16,
		flag_prepare_ptr /*! This will write the ptr value only without writing or reading any data. */ = I2C_FLAG_PREPARE_PTR,
		flag_prepare_data /*! This will read/write data without first writing the pointer information */ = I2C_FLAG_PREPARE_DATA,
		flag_is_slave_addr0 /*! If hardware supports multiple slave addrs, use the first slot (default) */ = I2C_FLAG_IS_SLAVE_ADDR0,
		flag_is_slave_addr1 /*! If hardware supports multiple slave addrs, use the second slot */ = I2C_FLAG_IS_SLAVE_ADDR1,
		flag_is_slave_addr2 /*! If hardware supports multiple slave addrs, use the third slot */ = I2C_FLAG_IS_SLAVE_ADDR2,
		flag_is_slave_addr3 /*! If hardware supports multiple slave addrs, use the fourth slot */ = I2C_FLAG_IS_SLAVE_ADDR3,
		flag_is_slave_ptr_8 /*! Use a 8-bit address pointer when accessing data (default) */ = I2C_FLAG_IS_SLAVE_PTR_8,
		flag_is_slave_ptr_16 /*! Use a 16-bit address pointer when accessing data (set automatically is size > 255) */ = I2C_FLAG_IS_SLAVE_PTR_16,
		flag_reset /*! Reset the state of the I2C */ = I2C_FLAG_RESET,
		flag_is_stretch_clock = I2C_FLAG_STRETCH_CLOCK,
		flag_is_no_stop /*! Don't issue a stop condition when complete (use with I2C_FLAG_PREPARE_DATA) */ = I2C_FLAG_IS_NO_STOP
	};

	enum error {
		error_none /*! No errors */ = I2C_ERROR_NONE,
		error_start /*! Error while starting */ = I2C_ERROR_START,
		error_write /*! Error while writing */ = I2C_ERROR_WRITE,
		error_ack /*! Ack Error (most common error for a mis-wired hardware) */ = I2C_ERROR_ACK,
		error_stop /*! Error while stopping */ = I2C_ERROR_STOP,
		error_master_ack /*! The master could not create an ACK */ = I2C_ERROR_MASTER_ACK,
		error_bus_busy /*! The Bus is busy (happens with multi-masters on bus) */ = I2C_ERROR_BUS_BUSY,
		error_long_slew = I2C_ERROR_LONG_SLEW,
		error_arbitration_lost /*! Arbitration lost on multi-master bus */ = I2C_ERROR_ARBITRATION_LOST
	};
};

API_OR_FLAGS_OPERATOR(I2CFlags)


/*! \brief I2C Pin Assignment
 * \details This class allows simple manipulation of the i2c_pin_assignment_t.
 *
 * \sa hal::I2C
 * \sa hal::I2CAttributes
 *
 */
class I2CPinAssignment : public PinAssignment<i2c_pin_assignment_t>{};


/*! \brief I2C Attr Class
 * \details The I2C attribute class is used for configuring and storing
 * I2C port settings.
 *
 * \sa hal::I2CPinAssignment
 * \sa hal::I2C
 *
 */
class I2CAttributes :
      public PinAssignmentPeriphAttributes<i2c_attr_t, i2c_pin_assignment_t>,
      public I2CFlags
{
public:

	I2CAttributes(){
		set_flags(flag_set_master);
		set_frequency(100000);
	}

	/*! \details Accesses the SDA pin assignment value. */
	mcu_pin_t sda() const { return m_attr.pin_assignment.sda; }
	/*! \details Access the SCL pin assignment value. */
	mcu_pin_t scl() const { return m_attr.pin_assignment.scl; }

	/*! \details Access the slave address value. */
	u8 slave_addr() const { return m_attr.slave_addr[0].addr8[0]; }

	/*! \details Accesses the 16-bit slave address value. */
	u8 slave_addr16() const { return m_attr.slave_addr[0].addr16; }


	I2CAttributes & set_flags(enum flags value){ m_attr.o_flags = value; return *this; }
	I2CAttributes & set_frequency(u32 value){ m_attr.freq = value; return *this; }

	/*! \details Sets the 7-bit slave address value. */
	I2CAttributes & set_slave_addr(u8 addr){ m_attr.slave_addr[0].addr8[0] = addr; return *this; }

	/*! \details Sets the 16-bit slave address value. */
	I2CAttributes & set_slave_addr16(u16 addr){ m_attr.slave_addr[0].addr16 = addr; return *this; }

	/*! \details Sets the SDA pin assignment value. */
	I2CAttributes & set_sda(const mcu_pin_t & pin){ m_attr.pin_assignment.sda = pin; return *this; }
	/*! \details Sets the SDA pin assignment value. */
	I2CAttributes & set_sda(u8 port, u8 pin){ m_attr.pin_assignment.sda = mcu_pin(port, pin); return *this; }

	/*! \details Sets the SCL pin assignment value. */
	I2CAttributes & set_scl(const mcu_pin_t & pin){ m_attr.pin_assignment.scl = pin; return *this; }
	/*! \details Sets the SCL pin assignment value. */
	I2CAttributes & set_scl(u8 port, u8 pin){ m_attr.pin_assignment.scl = mcu_pin(port, pin); return *this; }

};

typedef I2CAttributes I2CAttr;

/*! \brief I2C Peripheral Class
 * \details This class implements I2C device peripherals.
 *
 * An I2C ptr/data transfer (I2C::PREPARE_PTR_DATA) updates the register
 * pointer and then either reads or writes data.  For example:
 * \code
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	u16 data;
 * 	I2C i2c(0); //use I2C port 0
 * 	i2c.init(); //Initialize the I2C using the BSP defaults
 * 	i2c.prepare(0x4C); //slave addr is 0x4C - operation is 8-bit ptr plus data
 * 	i2c.read(5, &data, sizeof(data)); //read 2 bytes from register 5
 * 	i2c.close();
 * }
 * \endcode
 *
 * The code above will execute 2 I2C transactions. It will write 5 to the
 * I2C slave device register pointer. It will then read 2 bytes from the
 * slave device.
 *
 * The following example doesn't write the register pointer. It also shows
 * how to customize the initialization.
 *
 * \code
 * int main(int argc, char * argv[]){
 *	u16 data;
 * 	I2C i2c(1);
 * 	I2CPinAssignment pin_assignment;
 * 	pin_assignment->sda = mcu_pin(0,10);
 * 	pin_assignment->scl = mcu_pin(0,11);
 *
 * 	i2c.init(I2C::SET_MASTER, 400000, pin_assignment);
 * 	i2c.prepare(0x4C, FLAG_PREPARE_DATA); //prepare for data only transactions
 * 	i2c.write(&data, sizeof(u16)); //write two bytes
 * 	i2c.read(&data, sizeof(u16)); //read two bytes
 *	i2c.close();
 * }
 * \endcode
 *
 * The I2C can also be configured using POSIX calls.
 *
 * \code
 * int main(int argc, char * argv[]){
 * 	int fd;
 * 	fd = open("/dev/i2c0", O_RDWR);
 * 	if( fd < 0 ){
 * 		perror("Failed to open I2C");
 * 	} else {
 * 		i2c_attr_t attr;
 * 		attr.o_flags = I2C_FLAG_SET_MASTER;
 * 		attr.freq = 100000;
 * 		attr.pin_assignment.sda = mcu_pin(0,10);
 * 		attr.pin_assignment.scl = mcu_pin(0,11);
 * 		if( ioctl(fd, I_I2C_SETATTR, &attr) < 0 ){
 * 			perror("Failed to set I2C attr");
 *
 * 			//Or the BSP defaults can be used
 * 			if( ioctl(fd, I_I2C_SETATTR, 0) < 0 ){
 * 				perror("No BSP defaults");
 * 			}
 * 		} else {
 *
 * 			attr.o_flags = I2C_FLAG_PREPARE_PTR_DATA;
 * 			attr.slave_addr[0].addr8[0] = 0x4C;
 * 			ioctl(fd, I_I2C_SETATTR, &attr);
 * 			lseek(fd, 10, SEEK_SET);
 * 			write(fd, &value, sizeof(u16)); //first writes 10 (from lseek) then writes 2 bytes (value)
 * 		}
 * 		close(fd);
 * 	}
 *
 *	return 0;
 * }
 * \endcode
 *
 * \sa hal::I2CPinAssignment
 * \sa hal::I2CAttr
 *
 */
class I2C : public Periph<i2c_info_t, i2c_attr_t, I2CAttributes, 'i'>, public I2CFlags {
public:
	/*! \details Constructs an I2C object using the specified port. */
	I2C(port_t port);


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
	int prepare(u8 slave_addr, u32 o_flags = flag_prepare_ptr_data) const;

	/*! \details Resets the I2C bus state. */
	int reset() const;

	/*! \details Gets the last error. */
	int get_error() const;

	//deprecated
	API_DEPRECATED("use get_error()")
	int get_err() const { return get_error(); }

	int set_attr(u32 o_flags, u32 freq, const i2c_pin_assignment_t * pin_assignment = 0){
		i2c_attr_t attr;
		attr.o_flags = o_flags;
		attr.freq = freq;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(i2c_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(i2c_pin_assignment_t));
		}
		return set_attributes(attr);
	}

	int init(u32 o_flags, u32 freq, const i2c_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, pin_assignment);
	}


	using Periph::read;
	using Periph::write;


	/*! \details Sets (or clears) a specific bit in a a register
	 * on an I2C device.
	 *
	 * @param loc The register offset value
	 * @param bit The bit to set (or clear)
	 * @param high true to set the bit and false to clear it
	 * @return Zero on success
	 */
	int set(Location location, int bit, bool high = true);

	/*! \details Clears the bit in a register on an I2C device. */
	int clear(Location location, int bit){
		return set(location, bit, false);
	}

private:

};

}


#endif /* SAPI_HAL_I2C_HPP_ */
