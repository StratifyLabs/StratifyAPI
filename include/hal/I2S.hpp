#ifndef I2S_HPP
#define I2S_HPP

#include <sos/dev/i2s.h>
#include "Periph.hpp"
#include "PinAssignment.hpp"

namespace hal {

/*! \brief I2S Pin Assignment
 * \details This class allows simple manipulation of the i2c_pin_assignment_t.
 *
 * \sa hal::I2S
 * \sa hal::I2SAttr
 *
 */
class I2SPinAssignment : public PinAssignment<i2s_pin_assignment_t>{};

/*! \brief I2C Attr Class
 * \details The I2C attribute class is used for configuring and storing
 * I2C port settings.
 *
 * \sa hal::I2CPinAssignment
 * \sa hal::I2C
 *
 */
class I2SAttributes : public PinAssignmentPeriphAttributes<i2s_attr_t, i2s_pin_assignment_t> {
public:


	I2SAttributes(u32 o_flags = 0, u32 freq = 0, u32 mck_mult = 256){
		set_flags(o_flags);
		set_frequency(freq);
		m_attr.mck_mult = mck_mult;
	}

	/*! \details Accesses the serial data input pin assignment value. */
	mcu_pin_t sdin() const { return m_attr.pin_assignment.sdin; }
	/*! \details Access the serial data output pin assignment value. */
	mcu_pin_t sdout() const { return m_attr.pin_assignment.sdout; }

	/*! \details Sets the SDA pin assignment value. */
	void set_sdin(const mcu_pin_t & pin){ m_attr.pin_assignment.sdin = pin;}

	/*! \details Sets the SCL pin assignment value. */
	void set_sdout(const mcu_pin_t & pin){ m_attr.pin_assignment.sdout = pin;}

	/*! \details Sets the SCL pin assignment value. */
	void set_ws(const mcu_pin_t & pin){ m_attr.pin_assignment.ws = pin;}

	/*! \details Sets the SCL pin assignment value. */
	void set_sck(const mcu_pin_t & pin){ m_attr.pin_assignment.sck = pin;}

	/*! \details Sets the SCL pin assignment value. */
	void set_mck(const mcu_pin_t & pin){ m_attr.pin_assignment.mck = pin;}

	/*! \details Sets the frequency (rate for left/right clock). */
	void set_frequency(u32 frequency){ m_attr.freq = frequency; }
	void set_freq(u32 frequency){ set_frequency(frequency); }

	/*! \details Sets the o_flags value as specified. */
	void set_flags(u32 o_flags){ m_attr.o_flags = o_flags; }

};

typedef I2SAttributes I2SAttr;

class I2S : public Periph<i2s_info_t, i2s_attr_t, I2SAttributes, 'I'> {
public:
	I2S(port_t port);

	enum {
		IS_WIDTH_8 /*! I2S Word Width 8 bits */ = I2S_FLAG_IS_WIDTH_8,
		IS_WIDTH_16 /*! I2S Word Width 16 bits */ = I2S_FLAG_IS_WIDTH_16,
		IS_WIDTH_24 /*! I2S Word Width 24 bits */ = I2S_FLAG_IS_WIDTH_24,
		IS_WIDTH_32 /*! I2S Word Width 32 bits */ = I2S_FLAG_IS_WIDTH_32,
		IS_MONO /*! I2S Mono mode */ = I2S_FLAG_IS_MONO,
		IS_STEREO /*! I2S Stereo mode (default behavoir) */ = I2S_FLAG_IS_STEREO,
		SET_MASTER /*! Set the I2S as a master */ = I2S_FLAG_SET_MASTER,
		SET_SLAVE /*! Set the I2S as a slave */ = I2S_FLAG_SET_SLAVE,
		IS_TRANSMITTER /*! Set the I2S transmitter (master or slave) */ = I2S_FLAG_IS_TRANSMITTER,
		IS_RECEIVER /*! Set the I2S receiver (master or slave) */ = I2S_FLAG_IS_RECEIVER,
		IS_FORMAT_MSB /*! Set this bit for MSB format */ = I2S_FLAG_IS_FORMAT_MSB,
		IS_FORMAT_LSB /*! Set this bit for LSB format */ = I2S_FLAG_IS_FORMAT_LSB,
		IS_MCK_ENABLED /*! Set this bit to enable the mclk output */ = I2S_FLAG_IS_MCK_ENABLED,
		IS_FORMAT_PCM_SHORT /*! Set this bit for PCM Short format*/ = I2S_FLAG_IS_FORMAT_PCM_SHORT,
		IS_FORMAT_PCM_LONG /*! Set this bit for PCM Long format*/ = I2S_FLAG_IS_FORMAT_PCM_LONG,
		IS_WIDTH_16_EXTENDED /*! I2S has 16-bits of data in 32-bit blocks */ = I2S_FLAG_IS_WIDTH_16_EXTENDED

	};

	int set_attr(u32 o_flags, u32 freq, u32 mck_mult = 256, const i2s_pin_assignment_t * pin_assignment = 0){
		i2s_attr_t attr;
		attr.o_flags = o_flags;
		attr.mck_mult = mck_mult;
		attr.freq = freq;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(i2s_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(i2s_pin_assignment_t));
		}
		return set_attributes(attr);
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
	int init(u32 o_flags, u32 freq, u32 mck_mult = 256, const i2s_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, mck_mult, pin_assignment);
	}


private:


};

}

#endif // I2S_HPP
