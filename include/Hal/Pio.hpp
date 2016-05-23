/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_PIO_HPP_
#define STFY_APP_PIO_HPP_

#include <iface/dev/pio.h>
#include "Periph.hpp"

namespace Hal {


/*! \brief Pin Input/Output Class
 * \details This class controls pin input/output.
 *
 * \sa Hal::Pin
 *
 * Example:
 * \code
 *
 * #include <stfy/Hal.hpp>
 *
 * int main(int argc, char * argv[]){
 *
 *	//This allocates a PIO object but does not affect HW state
 * 	Pio pio(2); //use pio port 2
 *
 * 	//This opens the port and sets the attributes -- same as pio.open(); pio.setattr(1<<10, PIO_MODE_OUTPUT);
 * 	pio.init(1<<10, PIO_MODE_OUTPUT);
 *
 * 	//Bits can be manipulated using clrmask() and setmask()
 * 	pio.clrmask(1<<10); //clear bit 10 of port 2
 * 	pio.setmask(1<<10); //set bit 10
 *
 *	//the pin can be reconfigured as an input
 * 	pio.setattr(1<<10, PIO_MODE_INPUT | PIO_MODE_PULLDOWN); //set as an input with internal pulldown
 *
 * 	//get() is used to read the port
 * 	if( pio.get() & (1<<10) ){
 *		//bit 10 is set
 * 	}
 *
 * 	//This closes the port for all instances (ports are closed automatically when program exits)
 * 	pio.close();
 *
 * }
 *
 * \endcode
 */
class Pio : public Periph {
public:
	Pio(port_t port);

	enum {
		NONE /*! Specifies No PIO event */ = PIO_ACTION_EVENT_NONE,
		RISING /*! Specifies a rising edge on the PIO */ = PIO_ACTION_EVENT_RISING,
		FALLING /*! Specifies a falling edge on the PIO */ = PIO_ACTION_EVENT_FALLING,
		BOTH /*! Specifies both edges on the PIO */ = PIO_ACTION_EVENT_BOTH
	};

	/*! \details Get the attributes for the port */
	int attr(pio_attr_t * attr);
	/*! \details Set the attributes for the port */
	int set_attr(const pio_attr_t * attr);
	/*! \details Set the action for the port using pio_action_t */
	int set_action(const pio_action_t * action);

	/*! \details Set the action for an event
	 *
	 * @param channel The channel (pin) to fire the event
	 * @param event NONE, RISING, FALLING, or BOTH
	 * @param callback A privileged function to execute when the event happens (can be null) (return zero to delete the callback association)
	 * @param context The first argument passed to the callback
	 * @return Zero on success
	 */
	int set_action(u32 channel, u32 event, s8 prio = 0, mcu_callback_t callback = 0, void * context = 0){
		pio_action_t action;
		action.prio = prio;
		action.channel = channel;
		action.event = event;
		action.callback = callback;
		action.context = context;
		return set_action(&action);
	}

	/*! \details Set the specified pin mask */
	int set_mask(unsigned int mask);
	/*! \details Clear the specified mask */
	int clear_mask(unsigned int mask);
	/*! \details Get the value of the port */
	u32 value(void);

	/*! \details Set the value of the port */
	int set(unsigned int value);


	/*! \details Set the attributes for the port */
	int set_attr(pio_sample_t mask, int mode){
		pio_attr_t attr;
		attr.mask = mask;
		attr.mode = mode;
		return set_attr(&attr);
	}

	/*! \details This method opens the port
	 * then sets the attributes using the specified mask and mode.
	 * \code
	 * Pio p(0);
	 * p.init((1<<10)|(1<<5), PIO_MODE_OUTPUT)); //pins 0.5 and 0.10 are outputs
	 * \endcode
	 */
	int init(pio_sample_t mask, int mode){
		if( open() <  0 ){
			return -1;
		}
		return set_attr(mask, mode);
	}

	/*! \details PIO Pin mode types */
	enum {
		INPUT /*! Input mode */ = PIO_MODE_INPUT,
		OUTPUT /*! Output mode */ = PIO_MODE_OUTPUT,
		FLOAT /*! Input mode floating (| with INPUT) */ = PIO_MODE_FLOAT,
		PULLUP /*! Input mode pullup (| with INPUT) */ = PIO_MODE_PULLUP,
		PULLDOWN /*! Input mode pulldown (| with INPUT) */ = PIO_MODE_PULLDOWN,
		OPENDRAIN /*! Output mode open drain (| with OUTPUT) */ = PIO_MODE_OPENDRAIN,
		REPEATER /*! Input mode repeater (| with INPUT) */ = PIO_MODE_REPEATER,
		HYSTERESIS /*! Use hysteresis (| with INPUT) */ = PIO_MODE_HYSTERESIS
	};

#ifdef __MCU_ONLY__
	int close();
#endif

private:

};

};

#endif /* STFY_APP_PIO_HPP_ */
