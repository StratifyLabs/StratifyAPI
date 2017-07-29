/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_TMR_HPP_
#define SAPI_TMR_HPP_


#include <sos/dev/tmr.h>
#include "Periph.hpp"


namespace hal {

/*! \brief Tmr Pin Assignment
 * \details This class allows simple manipulation of the tmr_pin_assignment_t.
 */
class TmrPinAssignment : public PinAssignment<tmr_pin_assignment_t>{};


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
 * tmr.init(); //the default is 1MHz clock using the CPU see init() below
 * tmr.set(0); //set the value to zero
 * tmr.on(); //turn the timer on
 * Timer::wait_msec(100);
 * printf("My timer value is %d\n", tmr.value()); //The value will be about 100,000
 * \endcode
 *
 * Here is an example using the Tmr to count external edges on input capture 0
 *
 * \code
 * Tmr tmr(0);
 * tmr.init(0, //frequency is ignored in this case
 * 	Tmr::INPUT0|Tmr::RISING, //use input capture channel 0 rising edges as the clock source
 * 	__TMR0_PINASSIGN_PORT1_26_27_28_29, //This is for the LPC17xx to use P1.26 for CAP0.0 (timer0/input0)
 * 	0, //not using output compare channels
 * 	0); //not using input capture (other than clock source)
 * tmr.on();
 *  //Tmr0 is now counting edges arriving on P1.26 (CAP0.0) for LPC17xx devices
 * printf("Received %d edges\n", tmr.value()); //show the number of edges that have arrived
 * \endcode
 *
 * You can also use the output compare channels to set the top value of the timer.
 *
 * \code
 * Tmr tmr(0);
 * tmr.init(); //use default settings (1MHz CPU timer)
 *
 * tmr.set_action(0,
 * 	Tmr::RESET); //reset the timer to zero when the event happens
 * 	//Now assign a value to output compare channel 0
 * tmr.setoc(0, //we are using output compare channel 0
 * 	1000000 //When the count gets to 1 million (1 second at 1MHz), reset the timer
 * 	);
 * tmr.on();
 * \endcode
 *
 *
 */
class Tmr : public Periph<tmr_info_t, tmr_attr_t, 't'> {
public:
	Tmr(port_t port);

	/*! \details This lists the values for the event when using set_action().  The values
	 * can be Or'd (|) together such as Tmr::RESET|Tmr::INTERRUPT.
	 */
	enum {
		FLAG_IS_CLKSRC_CPU /*! Use the CPU as the source for the clock (timer mode) */ = TMR_FLAG_IS_CLKSRC_CPU,
		FLAG_IS_CLKSRC_IC0 /*! Use input capture channel 0 for the clock source (counter mode) */ = TMR_FLAG_IS_CLKSRC_IC0,
		FLAG_IS_CLKSRC_IC1 /*! Use input capture channel 1 for the clock source (counter mode) */ = TMR_FLAG_IS_CLKSRC_IC1,
		FLAG_IS_CLKSRC_IC2 /*! Use input capture channel 2 for the clock source (counter mode) */ = TMR_FLAG_IS_CLKSRC_IC2,
		FLAG_IS_CLKSRC_IC3 /*! Use input capture channel 3 for the clock source (counter mode) */ = TMR_FLAG_IS_CLKSRC_IC3,
		FLAG_IS_CLKSRC_EDGERISING /*! Count rising edges */ = TMR_FLAG_IS_CLKSRC_EDGERISING,
		FLAG_IS_CLKSRC_EDGEFALLING /*! Count falling edges */ = TMR_FLAG_IS_CLKSRC_EDGEFALLING,
		FLAG_IS_CLKSRC_EDGEBOTH /*! Count both edges */ = TMR_FLAG_IS_CLKSRC_EDGEBOTH,
		FLAG_IS_CLKSRC_COUNTDOWN /*! Count down (not up) */ = TMR_FLAG_IS_CLKSRC_COUNTDOWN,
		FLAG_AUTO_RELOAD /*! Auto reload the time */ = TMR_FLAG_AUTO_RELOAD,
		FLAG_ENABLE_OC /*! Auto reload the time */ = TMR_FLAG_ENABLE_OC,
		FLAG_ENABLE_IC /*! Auto reload the time */ = TMR_FLAG_ENABLE_IC,
		FLAG_SET_TIMER /*! Turn the timer on */ = TMR_FLAG_SET_TIMER,
		FLAG_SET_CHANNEL /*! Configure channel characteristics */ = TMR_FLAG_SET_CHANNEL,
		FLAG_IS_CHANNEL_STOP_ON_RESET /*! Stop when the timer resets */ = TMR_FLAG_IS_CHANNEL_STOP_ON_RESET,
		FLAG_IS_CHANNEL_RESET_ON_MATCH /*! Stop when the timer finds a match */ = TMR_FLAG_IS_CHANNEL_RESET_ON_MATCH,
		FLAG_IS_CHANNEL_STOP_ON_MATCH /*! Stop when the timer resets */ = TMR_FLAG_IS_CHANNEL_STOP_ON_MATCH,
		FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH /*! Stop when the timer resets */ = TMR_FLAG_IS_CHANNEL_SET_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH /*! Stop when the timer resets */ = TMR_FLAG_IS_CHANNEL_CLEAR_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH /*! Stop when the timer resets */ = TMR_FLAG_IS_CHANNEL_TOGGLE_OUTPUT_ON_MATCH,
		FLAG_IS_CHANNEL_PWM_MODE /*! Stop when the timer resets */ = TMR_FLAG_IS_CHANNEL_PWM_MODE,
	};

	/*! \details Turns the TMR on (start counting) */
	int enable() const;
	/*! \details Turns the TMR off (stop counting) */
	int disable() const;

	/*! \details Set the output compare attributes */
	int set_output_channel(u32 loc, u32 value){
		return set_channel(loc, value, I_TMR_SETOC);
	}

	u32 get_output_channel(u32 loc){
		return get_channel(loc, I_TMR_GETOC);
	}

	int set_input_channel(u32 loc, u32 value){
		return set_channel(loc, value, I_TMR_SETIC);
	}

	u32 get_input_channel(u32 loc){
		return get_channel(loc, I_TMR_GETIC);
	}

	u32 get_value() const;

	int set_value(u32 value) const;


private:

};

};

#endif /* SAPI_TMR_HPP_ */
