/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_HAL_TMR_HPP_
#define SAPI_HAL_TMR_HPP_


#include <sos/dev/tmr.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"


namespace hal {

/*! \brief Tmr Pin Assignment
 * \details This class allows simple manipulation of the tmr_pin_assignment_t.
 */
class TmrPinAssignment : public PinAssignment<tmr_pin_assignment_t>{};

class TmrAttributes : public PinAssignmentPeriphAttributes<tmr_attr_t, tmr_pin_assignment_t> {
public:

	TmrAttributes(u32 o_flags = 0, u32 freq = 0, u32 period = 0){
		set_flags(o_flags);
		set_frequency(freq);
		set_period(period);
	}


	mcu_pin_t channel(u8 channel) const {
		if( channel < 4 ){ return m_attr.pin_assignment.channel[channel]; }
		return mcu_pin(0xff, 0xff);
	}

	u32 period() const { return m_attr.period; }

	TmrAttributes & set_channel_pin(u8 channel, const mcu_pin_t & pin){
		if( channel < 4 ){
			m_attr.pin_assignment.channel[channel] = pin;
		}
		return *this;
	}

	TmrAttributes & set_period(u32 period){ m_attr.period = period; return *this; }
	TmrAttributes & set_channel(const mcu_channel_t & channel){
		m_attr.channel = channel;
		return *this;
	}

};

typedef TmrAttributes TmrAttr;

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
 * Timer::wait_milliseconds(100);
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
class Tmr : public Periph<tmr_info_t, tmr_attr_t, TmrAttributes, TMR_IOC_IDENT_CHAR> {
public:

	/*! \details Constructs a new timer object using \a port. */
	Tmr(port_t port);


	enum tmr_flags {
		/*! \cond */
		FLAG_SET_TIMER = TMR_FLAG_SET_TIMER,
		FLAG_IS_SOURCE_CPU = TMR_FLAG_IS_SOURCE_CPU,
		FLAG_IS_SOURCE_IC0 = TMR_FLAG_IS_SOURCE_IC0,
		FLAG_IS_SOURCE_IC1 = TMR_FLAG_IS_SOURCE_IC1,
		FLAG_IS_SOURCE_IC2 = TMR_FLAG_IS_SOURCE_IC2,
		FLAG_IS_SOURCE_IC3 = TMR_FLAG_IS_SOURCE_IC3,
		FLAG_IS_SOURCE_EDGERISING = TMR_FLAG_IS_SOURCE_EDGERISING,
		FLAG_IS_SOURCE_EDGEFALLING = TMR_FLAG_IS_SOURCE_EDGEFALLING,
		FLAG_IS_SOURCE_EDGEBOTH = TMR_FLAG_IS_SOURCE_EDGEBOTH,
		FLAG_IS_SOURCE_COUNTDOWN = TMR_FLAG_IS_SOURCE_COUNTDOWN,
		FLAG_IS_AUTO_RELOAD = TMR_FLAG_IS_AUTO_RELOAD,
		FLAG_SET_CHANNEL = TMR_FLAG_SET_CHANNEL,
		FLAG_IS_CHANNEL_STOP_ON_RESET = TMR_FLAG_IS_CHANNEL_STOP_ON_RESET,
		FLAG_IS_CHANNEL_RESET_ON_MATCH = TMR_FLAG_IS_CHANNEL_RESET_ON_MATCH,
		FLAG_IS_CHANNEL_STOP_ON_MATCH = TMR_FLAG_IS_CHANNEL_STOP_ON_MATCH,
		FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH = TMR_FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH = TMR_FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH = TMR_FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_PWM_MODE = TMR_FLAG_IS_CHANNEL_PWM_MODE,
		/*! \endcond */

		SET_TIMER /*! See \ref TMR_FLAG_SET_TIMER */ = TMR_FLAG_SET_TIMER,
		IS_SOURCE_CPU /*! See \ref TMR_FLAG_IS_SOURCE_CPU */ = TMR_FLAG_IS_SOURCE_CPU,
		IS_SOURCE_IC0 /*! See \ref TMR_FLAG_IS_SOURCE_IC0 */ = TMR_FLAG_IS_SOURCE_IC0,
		IS_SOURCE_IC1 /*! See \ref TMR_FLAG_IS_SOURCE_IC1 */ = TMR_FLAG_IS_SOURCE_IC1,
		IS_SOURCE_IC2 /*! See \ref TMR_FLAG_IS_SOURCE_IC2 */ = TMR_FLAG_IS_SOURCE_IC2,
		IS_SOURCE_IC3 /*! See \ref TMR_FLAG_IS_SOURCE_IC3 */ = TMR_FLAG_IS_SOURCE_IC3,
		IS_SOURCE_EDGERISING /*! See \ref TMR_FLAG_IS_SOURCE_EDGERISING */ = TMR_FLAG_IS_SOURCE_EDGERISING,
		IS_SOURCE_EDGEFALLING /*! See \ref TMR_FLAG_IS_SOURCE_EDGEFALLING */ = TMR_FLAG_IS_SOURCE_EDGEFALLING,
		IS_SOURCE_EDGEBOTH /*! See \ref TMR_FLAG_IS_SOURCE_EDGEBOTH */ = TMR_FLAG_IS_SOURCE_EDGEBOTH,
		IS_SOURCE_COUNTDOWN /*! See \ref TMR_FLAG_IS_SOURCE_COUNTDOWN */ = TMR_FLAG_IS_SOURCE_COUNTDOWN,
		IS_AUTO_RELOAD /*! See \ref TMR_FLAG_IS_AUTO_RELOAD */ = TMR_FLAG_IS_AUTO_RELOAD,
		SET_CHANNEL /*! See \ref TMR_FLAG_SET_CHANNEL */ = TMR_FLAG_SET_CHANNEL,
		IS_CHANNEL_STOP_ON_RESET /*! See \ref TMR_FLAG_IS_CHANNEL_STOP_ON_RESET */ = TMR_FLAG_IS_CHANNEL_STOP_ON_RESET,
		IS_CHANNEL_RESET_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_RESET_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_RESET_ON_MATCH,
		IS_CHANNEL_STOP_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_STOP_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_STOP_ON_MATCH,
		IS_CHANNEL_SET_OUTPUT_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH,
		IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH,
		IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH /*! See \ref TMR_FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH */ = TMR_FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH,
		IS_CHANNEL_PWM_MODE /*! See \ref TMR_FLAG_IS_CHANNEL_PWM_MODE */ = TMR_FLAG_IS_CHANNEL_PWM_MODE,
		SET_TRIGGER /*! See \ref TMR_FLAG_SET_TRIGGER */ = TMR_FLAG_SET_TRIGGER,
		IS_SLAVE /*! See \ref TMR_FLAG_IS_SLAVE */ = TMR_FLAG_IS_SLAVE,
		IS_MASTER /*! See \ref TMR_FLAG_IS_MASTER */ = TMR_FLAG_IS_MASTER,
		IS_TRIGGER_RELOAD /*! See \ref TMR_FLAG_IS_TRIGGER_RELOAD */ = TMR_FLAG_IS_TRIGGER_RELOAD,
		IS_TRIGGER_RESET /*! See \ref TMR_FLAG_IS_TRIGGER_RESET */ = TMR_FLAG_IS_TRIGGER_RESET,
		IS_TRIGGER_ENABLE /*! See \ref TMR_FLAG_IS_TRIGGER_ENABLE */ = TMR_FLAG_IS_TRIGGER_ENABLE,
		IS_TRIGGER_OC0 /*! See \ref TMR_FLAG_IS_TRIGGER_OC0 */ = TMR_FLAG_IS_TRIGGER_OC0,
		IS_TRIGGER_OC1 /*! See \ref TMR_FLAG_IS_TRIGGER_OC1 */ = TMR_FLAG_IS_TRIGGER_OC1,
		IS_TRIGGER_OC2 /*! See \ref TMR_FLAG_IS_TRIGGER_OC2 */ = TMR_FLAG_IS_TRIGGER_OC2,
		IS_TRIGGER_OC3 /*! See \ref TMR_FLAG_IS_TRIGGER_OC3 */ = TMR_FLAG_IS_TRIGGER_OC3,
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
		return Periph::set_channel(loc, value, arg::IoRequest(I_TMR_SETCHANNEL));
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
		return Periph::get_channel(loc, arg::IoRequest(I_TMR_GETCHANNEL));
	}

	/*! \details Gets the value of the timer. */
	u32 get_value() const;

	/*! \details Sets the value of the timer. */
	int set_value(u32 value) const;


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
		return set_attributes(attr);
	}

	int init(u32 o_flags, u32 freq, u32 period, const tmr_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, period, pin_assignment);
	}


private:

};

}

#endif /* SAPI_HAL_TMR_HPP_ */
