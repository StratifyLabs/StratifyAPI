#ifndef SAPI_HAL_I2S_HPP_
#define SAPI_HAL_I2S_HPP_

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
/*! \brief SAI Pin Assignment
 * \details This class allows simple manipulation of the sai_pin_assignment_t.
 *
 * \sa hal::I2S
 * \sa hal::SAIAttr
 *
 */
class SAIPinAssignment : public PinAssignment<sai_pin_assignment_t>{};

/*! \brief I2S Attr Class
 * \details The I2S attribute class is used for configuring and storing
 * I2S port settings.
 *
 * \sa hal::I2SPinAssignment
 * \sa hal::I2S
 *
 */
class I2SAttributes : public PinAssignmentPeriphAttributes<i2s_attr_t, i2s_pin_assignment_t> {
public:
	I2SAttributes(){
		set_flags(I2S_FLAG_SET_MASTER);
		set_frequency(16000);
		m_attr.mck_mult = 256;
	}

	/*! \details Accesses the serial data input pin assignment value. */
	mcu_pin_t sdin() const { return m_attr.pin_assignment.sdin; }
	/*! \details Access the serial data output pin assignment value. */
	mcu_pin_t sdout() const { return m_attr.pin_assignment.sdout; }

	/*! \details Sets the SDA pin assignment value. */
	I2SAttributes & set_sdin(const mcu_pin_t & pin){ m_attr.pin_assignment.sdin = pin; return *this; }

	/*! \details Sets the SCL pin assignment value. */
	I2SAttributes & set_sdout(const mcu_pin_t & pin){ m_attr.pin_assignment.sdout = pin; return *this; }

	/*! \details Sets the SCL pin assignment value. */
	I2SAttributes & set_ws(const mcu_pin_t & pin){ m_attr.pin_assignment.ws = pin; return *this; }

	/*! \details Sets the SCL pin assignment value. */
	I2SAttributes & set_sck(const mcu_pin_t & pin){ m_attr.pin_assignment.sck = pin; return *this; }

	/*! \details Sets the SCL pin assignment value. */
	I2SAttributes & set_mck(const mcu_pin_t & pin){ m_attr.pin_assignment.mck = pin; return *this; }

	/*! \details Sets the frequency (rate for left/right clock). */
	I2SAttributes & set_frequency(u32 frequency){ m_attr.freq = frequency; return *this; }

	/*! \details Sets the o_flags value as specified. */
	I2SAttributes & set_flags(u32 o_flags){ m_attr.o_flags = o_flags; return *this; }

};

typedef I2SAttributes I2SAttr;

/*! \brief SAI Attr Class
 * \details The SAI attribute class is used for configuring and storing
 * SAI port settings.
 *
 * \sa hal::SAIPinAssignment
 * \sa hal::I2S
 *
 */
class SAIAttributes : public PinAssignmentPeriphAttributes<sai_attr_t, sai_pin_assignment_t> {
public:
	mcu_pin_t sck;    /*!< in/out bit clock*/
	mcu_pin_t sd;     /*!< data line*/
	mcu_pin_t mclk; /*!< output master clock not used in slave mode*/
	mcu_pin_t fs;   /*!< frame synchronization*/

	SAIAttributes(u32 o_flags = 0, u32 freq = 0, u32 mck_mult = 256){
		set_flags(o_flags);
		set_frequency(freq);
		m_attr.mck_mult = mck_mult;
	}

	/*! \details Accesses the serial data line pin assignment value. */
	mcu_pin_t sd_pin() const { return m_attr.pin_assignment.sd; }
	/*! \details Access the serial in/out bit clock pin assignment value. */
	mcu_pin_t sck_pin() const { return m_attr.pin_assignment.sck; }
	/*! \details Access the FS pin assignment value - frame synchronization. */
	mcu_pin_t fs_pin() const { return m_attr.pin_assignment.fs; }
	/*! \details Sets the mclk pin assignment value - output master clock not used in slave mode. */
	mcu_pin_t mclk_pin() const { return m_attr.pin_assignment.mclk; }

	/*! \details Sets the SD pin assignment value - data line. */
	void set_sd(const mcu_pin_t & pin){ m_attr.pin_assignment.sd = pin;}

	/*! \details Sets the SCK  pin assignment value - in/out bit clock. */
	void set_sck(const mcu_pin_t & pin){ m_attr.pin_assignment.sck = pin;}

	/*! \details Sets the FS pin assignment value - frame synchronization. */
	void set_fs(const mcu_pin_t & pin){ m_attr.pin_assignment.fs = pin;}

	/*! \details Sets the mclk pin assignment value - output master clock not used in slave mode. */
	void set_mclk(const mcu_pin_t & pin){ m_attr.pin_assignment.mclk = pin;}

	/*! \details Sets the frequency (rate for left/right clock). */
	void set_frequency(u32 frequency){ m_attr.freq = frequency; }
	void set_freq(u32 frequency){ set_frequency(frequency); }

	/*! \details Sets the o_flags value as specified. */
	void set_flags(u32 o_flags){ m_attr.o_flags = o_flags; }

};

typedef SAIAttributes SAIAttr;

class I2S : public Periph<i2s_info_t, i2s_attr_t, I2SAttributes, 'I'> {
public:
	I2S(port_t port);

	enum i2s_flags {
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

private:


};

/*! \cond */
class SAI : public Periph<i2s_info_t, sai_attr_t, SAIAttributes, 'I'> {
public:
	SAI(port_t port);

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
		IS_WIDTH_16_EXTENDED /*! I2S has 16-bits of data in 32-bit blocks */ = I2S_FLAG_IS_WIDTH_16_EXTENDED,
		IS_OUTPUTDRIVE_DISABLE /*! SAI_Block_Output_Drive SAI Block Output Drive*/ = SAI_FLAG_IS_OUTPUTDRIVE_DISABLE,
		IS_FIFOTHRESHOLD_EMPTY /*! SAI_Block_Fifo_Threshold SAI Block Fifo Threshold*/ = SAI_FLAG_IS_FIFOTHRESHOLD_EMPTY,
		IS_FIFOTHRESHOLD_1QF /*! SAI_Block_Fifo_Threshold SAI Block Fifo Threshold*/ = SAI_FLAG_IS_FIFOTHRESHOLD_1QF,
		IS_FIFOTHRESHOLD_HF /*! SAI_Block_Fifo_Threshold SAI Block Fifo Threshold*/ = SAI_FLAG_IS_FIFOTHRESHOLD_HF,
		IS_FIFOTHRESHOLD_3QF /*! SAI_Block_Fifo_Threshold SAI Block Fifo Threshold*/ = SAI_FLAG_IS_FIFOTHRESHOLD_3QF,
		IS_FIFOTHRESHOLD_FULL /*! SAI_Block_Fifo_Threshold SAI Block Fifo Threshold*/ = SAI_FLAG_IS_FIFOTHRESHOLD_FULL,
		IS_ULAW_1CPL_COMPANDING /*! SAI Block Companding Mode*/ = SAI_FLAG_IS_ULAW_1CPL_COMPANDING,
		IS_ALAW_1CPL_COMPANDING /*! SAI Block Companding Mode*/ = SAI_FLAG_IS_ALAW_1CPL_COMPANDING,
		IS_ULAW_2CPL_COMPANDING /*! SAI Block Companding Mode*/ = SAI_FLAG_IS_ULAW_2CPL_COMPANDING,
		IS_ALAW_2CPL_COMPANDING /*! SAI Block Companding Mode*/ = SAI_FLAG_IS_ALAW_2CPL_COMPANDING,
		IS_SAI_FLAG_ENABLE       /*! SAI enable command*/ = SAI_FLAG_ENABLE,
		IS_SAI_DMA_ENABLE       /*! SAI dma enable command*/ = SAI_DMA_ENABLE,
		IS_SYNCHRONOUS /*! Synchronous with other block of same SAI */ = SAI_FLAG_IS_SYNCHRONOUS,
		IS_SYNCHRONOUS_EXT_SAI1 /*! Synchronous with other SAI, SAI1 */ = SAI_FLAG_IS_SYNCHRONOUS_EXT_SAI1,
		IS_SYNCHRONOUS_EXT_SAI2 /*! Synchronous with other SAI, SAI2 */ = SAI_FLAG_IS_SYNCHRONOUS_EXT_SAI2,
		SET_SLOT/*!< update active slot*/ = SAI_FLAG_SET_SLOT
	};

	int set_attr(u32 o_flags, u32 freq,u32 slot, u32 mck_mult = 1, const sai_pin_assignment_t * pin_assignment = 0){
		sai_attr_t attr;
		attr.o_flags = o_flags;
		attr.mck_mult = mck_mult;
		attr.freq = freq;
		attr.slot = slot;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(sai_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(sai_pin_assignment_t));
		}
		return set_attributes(attr);
	}

	/*! \details This method initializes the SAI port - open and set attribute.
	  *
	  * @param o_flags Flag bitmask
	  * @param freq Bitrate
	  * @param pin_assignment Pin assignment or null to use default pin assignment
	  * @return Zero on success
	  *
	  * This method calls open() then set_attr().
	  *
	  */
	int init(u32 o_flags, u32 freq,u32 slot, u32 mck_mult = 1, const sai_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, slot, mck_mult, pin_assignment);
	}
	int set_action(u8 channel, 	mcu_callback_t callback,
						void * context = NULL,s8 prio = 3){
		mcu_action_t action ;
		action.channel = channel;
		action.prio = prio;
		action.handler.callback = callback;
		action.handler.context = context;
		return ioctl(I_MCU_SETACTION, &action);
	}



private:

};
/*! \endcond */


}

#endif // SAPI_HAL_I2S_HPP_
