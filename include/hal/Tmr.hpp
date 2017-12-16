/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_TMR_HPP_
#define SAPI_TMR_HPP_


#include <sos/dev/tmr.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"


namespace hal {

/*! \brief Tmr Pin Assignment
 * \details This class allows simple manipulation of the tmr_pin_assignment_t.
 */
class TmrPinAssignment : public PinAssignment<tmr_pin_assignment_t>{};

class TmrAttr {
public:

	TmrAttr(){
		m_port = 0;
		memset(&m_attr, 0, sizeof(m_attr));
		memset(&m_attr.pin_assignment, 0xff, sizeof(tmr_pin_assignment_t));
	}

	u8 port() const { return m_port; }
	operator tmr_attr_t() const { return m_attr; }
	const tmr_attr_t & attr() const { return m_attr; }
	mcu_pin_t channel(u8 channel) const {
		if( channel < 4 ){ return m_attr.pin_assignment.channel[channel]; }
		return mcu_pin(0xff, 0xff);
	}
	u32 freq() const { return m_attr.freq; }
	u32 period() const { return m_attr.period; }

	void set_channel_pin(u8 channel, const mcu_pin_t & pin){
		if( channel < 4 ){
			m_attr.pin_assignment.channel[channel] = pin;
		}
	}
	void set_port(u8 p){ m_port = p; }
	void set_flags(u32 flags){ m_attr.o_flags = flags; }
	void set_freq(u32 frequency){ m_attr.freq = frequency; }
	void set_period(u32 period){ m_attr.period = period; }
	void set_channel(const mcu_channel_t & channel){
		m_attr.channel = channel;
	}

private:
	u8 m_port;
	tmr_attr_t m_attr;
};

/*! \brief Tmr (hardware timer) Class
 * \details This class implements a hardware timer.  The Timer class implements
 * a logical timer that is based on the system clock and can be used for many timing functions.
 *
 * The Tmr class can be used to configure a timer to count rising edges on an input capture pin
 * or to generate waveforms on an output compare pin.  It can also configure the timer
 * to use the internal CPU clock at various frequencies.
 *
 * For example this will start a microsecond timer (1Mhz) using an internal clock:
 *
 * \code
 * Tmr tmr(0); //use /dev/tmr0
 * tmr.init(); //initialize the timer
 * tmr.set(0); //set the value to zero
 * tmr.on(); //turn the timer on
 * Timer::wait_msec(100);
 * printf("My timer value is %d\n", tmr.value()); //The value will be about 100,000
 * \endcode
 *
 * Here is an example using the Tmr to count external edges on input capture 0
 *
 * \code
 * \endcode
 *
 * You can also use the output compare channels to set the top value of the timer.
 *
 * \code
 * \endcode
 *
 *
 */
class Tmr : public Periph<tmr_info_t, tmr_attr_t, 't'> {
public:

	/*! \details Constructs a new timer object using \a port. */
	Tmr(port_t port);


	enum {
		FLAG_SET_TIMER /*! See \ref TMR_FLAG_SET_TIMER */ = TMR_FLAG_SET_TIMER,
		FLAG_IS_SOURCE_CPU /*! See \ref TMR_FLAG_IS_SOURCE_CPU */ = TMR_FLAG_IS_SOURCE_CPU,
		FLAG_IS_SOURCE_IC0 /*! See \ref TMR_FLAG_IS_SOURCE_IC0 */ = TMR_FLAG_IS_SOURCE_IC0,
		FLAG_IS_SOURCE_IC1 /*! See \ref TMR_FLAG_IS_SOURCE_IC1 */ = TMR_FLAG_IS_SOURCE_IC1,
		FLAG_IS_SOURCE_IC2 /*! See \ref TMR_FLAG_IS_SOURCE_IC2 */ = TMR_FLAG_IS_SOURCE_IC2,
		FLAG_IS_SOURCE_IC3 /*! See \ref TMR_FLAG_IS_SOURCE_IC3 */ = TMR_FLAG_IS_SOURCE_IC3,
		FLAG_IS_SOURCE_EDGERISING /*! See \ref TMR_FLAG_IS_SOURCE_EDGERISING */ = TMR_FLAG_IS_SOURCE_EDGERISING,
		FLAG_IS_SOURCE_EDGEFALLING /*! See \ref TMR_FLAG_IS_SOURCE_EDGEFALLING */ = TMR_FLAG_IS_SOURCE_EDGEFALLING,
		FLAG_IS_SOURCE_EDGEBOTH /*! See \ref TMR_FLAG_IS_SOURCE_EDGEBOTH */ = TMR_FLAG_IS_SOURCE_EDGEBOTH,
		FLAG_IS_SOURCE_COUNTDOWN /*! See \ref TMR_FLAG_IS_SOURCE_COUNTDOWN */ = TMR_FLAG_IS_SOURCE_COUNTDOWN,
		FLAG_IS_AUTO_RELOAD /*! See \ref TMR_FLAG_IS_AUTO_RELOAD */ = TMR_FLAG_IS_AUTO_RELOAD,
		FLAG_SET_CHANNEL /*! See \ref TMR_FLAG_SET_CHANNEL */ = TMR_FLAG_SET_CHANNEL,
		FLAG_IS_CHANNEL_STOP_ON_RESET /*! See \ref TMR_FLAG_IS_CHANNEL_STOP_ON_RESET */ = TMR_FLAG_IS_CHANNEL_STOP_ON_RESET,
		FLAG_IS_CHANNEL_RESET_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_RESET_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_RESET_ON_MATCH,
		FLAG_IS_CHANNEL_STOP_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_STOP_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_STOP_ON_MATCH,
		FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_PWM_MODE /*! See \ref TMR_FLAG_IS_CHANNEL_PWM_MODE */ = TMR_FLAG_IS_CHANNEL_PWM_MODE,
	};

	/*! \details Turns the TMR on (start counting) */
	int enable() const;

	/*! \details Turns the TMR off (stop counting) */
	int disable() const;

	/*! \details Sets the value of the specified channel.
	 *
	 * @param loc The channel number
	 * @param value The value to assign to the channel
	 * @return Zero on success
	 *
	 * The \a loc parameter is the input or output channel. Input channels
	 * should be or'd with the MCU_CHANNEL_FLAG_IS_INPUT value.
	 *
	 */
	int set_channel(u32 loc, u32 value){
		return Periph::set_channel(loc, value, I_TMR_SETCHANNEL);
	}

	/*! \details Gets the value of the specified channel.
	 *
	 * @param loc The channel number
	 * @return The value of the channel or (u32)-1 if the IOCTL request fails
	 *
	 * The \a loc parameter is the input or output channel. Input channels
	 * should be or'd with the MCU_CHANNEL_FLAG_IS_INPUT value.
	 *
	 */
	u32 get_channel(u32 loc){
		return Periph::get_channel(loc, I_TMR_GETCHANNEL);
	}

	/*! \details Gets the value of the timer. */
	u32 get_value() const;

	/*! \details Sets the value of the timer. */
	int set_value(u32 value) const;


	/*! \details Sets the hardware timer attributes as specified.
	 *
	 * @param o_flags The Tmr flags
	 * @param freq The timer frequency (if applicable)
	 * @param period The timer period (if applicable and supported)
	 * @param pin_assignment A pointer to the pin assignment
	 * @return Zero on success
	 */
	int set_attr(u32 o_flags, u32 freq, u32 period, const tmr_pin_assignment_t * pin_assignment = 0){
		tmr_attr_t attr;
		attr.o_flags = o_flags;
		attr.freq = freq;
		attr.period = period;
		if( pin_assignment ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(uart_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(uart_pin_assignment_t));
		}
		return set_attr(attr);
	}

	/*! \details Initializes the hardware timer as specified.
	 *
	 * @param o_flags The Tmr flags
	 * @param freq The timer frequency (if applicable)
	 * @param period The timer period (if applicable and supported)
	 * @param pin_assignment A pointer to the pin assignment
	 * @return Zero on success
	 *
	 * This method will first open the port and then set the timer attributes.
	 */
	int init(u32 o_flags, u32 freq, u32 period, const tmr_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, period, pin_assignment);
	}


	using Periph::init;
	using Periph::set_attr;

private:

};

};

#endif /* SAPI_TMR_HPP_ */
