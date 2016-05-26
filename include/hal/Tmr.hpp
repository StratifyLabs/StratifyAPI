/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_TMR_HPP_
#define STFY_APP_TMR_HPP_


#include <hal/Periph.hpp>
#include <iface/dev/tmr.h>


namespace hal {


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
class Tmr : public Periph {
public:
	Tmr(port_t port);
	/*! \details Get the TMR attributes */
	int get_attr(tmr_attr_t * attr);
	/*! \details Set the TMR attributes */
	int set_attr(const tmr_attr_t * attr);

	/*! \details This lists the values for the event when using set_action().  The values
	 * can be Or'd (|) together such as Tmr::RESET|Tmr::INTERRUPT.
	 */
	enum {
		NONE = /*! No Event */ TMR_ACTION_EVENT_NONE,
		INTERRUPT /*! Trigger interrupt and execute callback */ = TMR_ACTION_EVENT_INTERRUPT,
		RESET /*! Reset timer value to zero */ = TMR_ACTION_EVENT_RESET,
		STOP /*! Stop the timer */ = TMR_ACTION_EVENT_STOP,
		SETOC /*! Set the output compare pin high */ = TMR_ACTION_EVENT_SETOC,
		CLROC /*! Clear the output compare pin */ = TMR_ACTION_EVENT_CLROC,
		TOGGLEOC /*! Toggle the output compare pin */ = TMR_ACTION_EVENT_TOGGLEOC
	};


	/*! \details Set the TMR action */
	int set_action(const tmr_action_t * action);
	/*! \details Set the TMR action */
	int set_action(int channel /*! The channel to use */,
			int event /*! The event such as Tmr::INTERRUPT */,
			mcu_callback_t callback /*! Callback executed if Tmr::INTERRUPT is used */ =0,
			void * context /*! First argument passed to callback */ = 0){
		tmr_action_t action;
		action.channel = channel;
		action.event = event;
		action.callback = callback;
		action.context = context;
		return set_action(&action);
	}
	/*! \details Turn the TMR on (start counting) */
	int on();
	/*! \details Turn the TMR off (stop counting) */
	int off();
	/*! \details Set the output compare attributes */
	int set_output_compare(const tmr_reqattr_t * req);
	/*! \details Set the output compare unit with given parameters */
	int set_output_compare(int channel, uint32_t value){
		tmr_reqattr_t req;
		req.channel = channel;
		req.value = value;
		return set_output_compare(&req);
	}
	/*! \details Get the output compare attributes */
	int get_output_compare(tmr_reqattr_t * req);
	/*! \details Get the output compare attributes (no error checking) */
	int get_output_compare(int channel){
		tmr_reqattr_t req;
		req.channel = channel;
		get_output_compare(&req);
		return req.value;
	}
	/*! \details Set the input capture attributes */
	int set_input_capture(const tmr_reqattr_t * req);
	/*! \details Set the input capture unit with given parameters */
	int set_input_capture(int channel, uint32_t value){
		tmr_reqattr_t req;
		req.channel = channel;
		req.value = value;
		return set_input_capture(&req);
	}
	/*! \details Get the input capture attributes */
	int get_input_capture(tmr_reqattr_t * req);
	/*! \details Get the input capture value (no error checking)*/
	int get_input_capture(int channel){
		tmr_reqattr_t req;
		req.channel = channel;
		get_input_capture(&req);
		return req.value;
	}
	/*! \details Return the value of the TMR */
	tmr_sample_t get_value();

	/*! \details Set the value of the TMR */
	int set_value(tmr_sample_t value);

#ifdef __MCU_ONLY__
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int close();
#endif

	/*! \details See init() for details. */
	int set_attr(uint32_t freq,
			uint8_t clksrc = TMR_CLKSRC_CPU,
			uint8_t pin_assign = 0,
			uint8_t enabled_oc_chans = 0,
			uint8_t enabled_ic_chans = 0){
		tmr_attr_t attr;
		attr.clksrc = clksrc;
		attr.freq = freq;
		attr.pin_assign = pin_assign;
		attr.enabled_ic_chans = enabled_ic_chans;
		attr.enabled_oc_chans = enabled_oc_chans;
		return set_attr(&attr);
	}

	/*! \details This is a list of the timer clock sources.  Not all clock
	 * sources are available on all devices.
	 */
	enum {
		CPU /*! Use the internal CPU clock */ = TMR_CLKSRC_CPU,
		INPUT0 /*! Use input 0 (device specific) */ = TMR_CLKSRC_IC0,
		INPUT1 /*! Use input 1 (device specific) */ = TMR_CLKSRC_IC1,
		INPUT2 /*! Use input 2 (device specific) */ = TMR_CLKSRC_IC2,
		INPUT3 /*! Use input 3 (device specific) */ = TMR_CLKSRC_IC3,
		RISING /*! Modifier to clock on rising edges */ = TMR_CLKSRC_EDGERISING,
		FALLING /*! Modifier to clock on falling edges */ = TMR_CLKSRC_EDGEFALLING,
		BOTH /*! Modifier to clock on both rising and falling edges */ = TMR_CLKSRC_EDGEBOTH,
		COUNTDOWN /*! Modifier for a countdown timer */ = TMR_CLKSRC_COUNTDOWN
	};

	/*! \details This opens and sets the timer attributes
	 *
	 */
	int init(uint32_t freq /*! timer frequency (if using TMR_CLKSRC_CPU) */ = 1000000,
			uint8_t clksrc /*! clock source for the timer (default is Tmr::CPU) */ = TMR_CLKSRC_CPU,
			uint8_t pin_assign /*! The pin assignment if using input capture (including clock source) or output compare pins */ = 0,
			uint8_t enabled_oc_chans /*! Mask of the enabled output compare channels (only if external pins are used) */ = 0,
			uint8_t enabled_ic_chans /*! Mask of the enabled input capture channels (only if external pins are used--not including clock source) */ = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(freq, clksrc, pin_assign, enabled_oc_chans, enabled_ic_chans);
	}

private:

};

};

#endif /* STFY_APP_TMR_HPP_ */
