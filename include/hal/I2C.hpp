/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_I2C_HPP_
#define SAPI_I2C_HPP_

#include <sos/dev/i2c.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"



namespace hal {

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
class I2CAttributes : public PinAssignmentPeriphAttributes<i2c_attr_t, i2c_pin_assignment_t> {
public:

	I2CAttributes(u32 o_flags = I2C_FLAG_SET_MASTER, u32 freq = 100000){
		set_flags(o_flags);
		set_freq(freq);
	}

	/*! \details Accesses the SDA pin assignment value. */
	mcu_pin_t sda() const { return m_attr.pin_assignment.sda; }
	/*! \details Access the SCL pin assignment value. */
	mcu_pin_t scl() const { return m_attr.pin_assignment.scl; }

	/*! \details Access the slave address value. */
	u8 slave_addr() const { return m_attr.slave_addr[0].addr8[0]; }

	/*! \details Accesses the 16-bit slave address value. */
	u8 slave_addr16() const { return m_attr.slave_addr[0].addr16; }

	/*! \details Sets the 7-bit slave address value. */
	void set_slave_addr(u8 addr){ m_attr.slave_addr[0].addr8[0] = addr; }

	/*! \details Sets the 16-bit slave address value. */
	void set_slave_addr16(u16 addr){ m_attr.slave_addr[0].addr16 = addr; }

	/*! \details Sets the SDA pin assignment value. */
	void set_sda(const mcu_pin_t & pin){ m_attr.pin_assignment.sda = pin;}
	/*! \details Sets the SDA pin assignment value. */
	void set_sda(u8 port, u8 pin){ m_attr.pin_assignment.sda = mcu_pin(port, pin);}

	/*! \details Sets the SCL pin assignment value. */
	void set_scl(const mcu_pin_t & pin){ m_attr.pin_assignment.scl = pin;}
	/*! \details Sets the SCL pin assignment value. */
	void set_scl(u8 port, u8 pin){ m_attr.pin_assignment.scl = mcu_pin(port, pin);}

};

typedef I2CAttributes I2CAttr;

/*! \brief I2C Peripheral Class
 * \details This class implements I2C device peripherals.
 *
 * An I2C ptr/data transfer (I2C::FLAG_PREPARE_PTR_DATA) updates the register
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
 * 	i2c.init(I2C::FLAG_SET_MASTER, 400000, pin_assignment);
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
class I2C : public Periph<i2c_info_t, i2c_attr_t, I2CAttributes, 'i'> {
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
		FLAG_SET_MASTER = I2C_FLAG_SET_MASTER,
		FLAG_SET_SLAVE = I2C_FLAG_SET_SLAVE,
		FLAG_IS_SLAVE_ACK_GENERAL_CALL = I2C_FLAG_IS_SLAVE_ACK_GENERAL_CALL,
		FLAG_IS_PULLUP = I2C_FLAG_IS_PULLUP,
		FLAG_PREPARE_PTR_DATA = I2C_FLAG_PREPARE_PTR_DATA,
		FLAG_IS_PTR_16 = I2C_FLAG_IS_PTR_16,
		FLAG_PREPARE_PTR = I2C_FLAG_PREPARE_PTR,
		FLAG_PREPARE_DATA = I2C_FLAG_PREPARE_DATA,
		FLAG_IS_SLAVE_ADDR0 = I2C_FLAG_IS_SLAVE_ADDR0,
		FLAG_IS_SLAVE_ADDR1 = I2C_FLAG_IS_SLAVE_ADDR1,
		FLAG_IS_SLAVE_ADDR2 = I2C_FLAG_IS_SLAVE_ADDR2,
		FLAG_IS_SLAVE_ADDR3 = I2C_FLAG_IS_SLAVE_ADDR3,
		FLAG_IS_SLAVE_PTR_8 = I2C_FLAG_IS_SLAVE_PTR_8,
		FLAG_IS_SLAVE_PTR_16 = I2C_FLAG_IS_SLAVE_PTR_16,
		FLAG_RESET = I2C_FLAG_RESET,
		FLAG_STRETCH_CLOCK = I2C_FLAG_STRETCH_CLOCK,
		FLAG_IS_NO_STOP = I2C_FLAG_IS_NO_STOP,

		SET_MASTER /*! Operate as a master I2C bus */ = I2C_FLAG_SET_MASTER,
		SET_SLAVE/*! Operate as a slave (ignored if master is set) */ = I2C_FLAG_SET_SLAVE,
		IS_SLAVE_ACK_GENERAL_CALL /*! If slave operation, ack general call */ = I2C_FLAG_IS_SLAVE_ACK_GENERAL_CALL,
		IS_PULLUP /*! Enable internal pullups if available (ignore otherwise) */ = I2C_FLAG_IS_PULLUP,
		PREPARE_PTR_DATA /*! This prepares the driver to write the ptr then read/write data */ = I2C_FLAG_PREPARE_PTR_DATA,
		IS_PTR_16 /*! This prepares the driver to write a 16-bit ptr then read/write data */ = I2C_FLAG_IS_PTR_16,
		PREPARE_PTR /*! This will write the ptr value only without writing or reading any data. */ = I2C_FLAG_PREPARE_PTR,
		PREPARE_DATA /*! This will read/write data without first writing the pointer information */ = I2C_FLAG_PREPARE_DATA,
		IS_SLAVE_ADDR0 /*! If hardware supports multiple slave addrs, use the first slot (default) */ = I2C_FLAG_IS_SLAVE_ADDR0,
		IS_SLAVE_ADDR1 /*! If hardware supports multiple slave addrs, use the second slot */ = I2C_FLAG_IS_SLAVE_ADDR1,
		IS_SLAVE_ADDR2 /*! If hardware supports multiple slave addrs, use the third slot */ = I2C_FLAG_IS_SLAVE_ADDR2,
		IS_SLAVE_ADDR3 /*! If hardware supports multiple slave addrs, use the fourth slot */ = I2C_FLAG_IS_SLAVE_ADDR3,
		IS_SLAVE_PTR_8 /*! Use a 8-bit address pointer when accessing data (default) */ = I2C_FLAG_IS_SLAVE_PTR_8,
		IS_SLAVE_PTR_16 /*! Use a 16-bit address pointer when accessing data (set automatically is size > 255) */ = I2C_FLAG_IS_SLAVE_PTR_16,
		RESET /*! Reset the state of the I2C */ = I2C_FLAG_RESET,
		IS_STRETCH_CLOCK = I2C_FLAG_STRETCH_CLOCK,
		IS_NO_STOP /*! Don't issue a stop condition when complete (use with I2C_FLAG_PREPARE_DATA) */ = I2C_FLAG_IS_NO_STOP
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

	/*! \details Resets the I2C bus state. */
	int reset() const;

	/*! \details Gets the last error. */
	int get_error() const;

	//deprecated
#if !defined __link
	[[deprecated("use get_error()")]]
#endif
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

	/*! \details Reads the value of a register on an I2C device */
	int read(int loc, u8 & reg);
	/*! \details Writes the value of a register on an I2C device */
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

}


#endif /* SAPI_I2C_HPP_ */
