/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_EINT_HPP_
#define STFY_APP_EINT_HPP_


#include <iface/dev/eint.h>
#include "Periph.hpp"
#include "Pin.hpp"


namespace hal {

/*! \brief External Interrupt Class
 * \details This class gives access to external interrupt circuitry.  You can use this class
 * to have the interrupt trigger a function or block a thread until the interrupt arrives.
 *
 * Here is an example:
 *
 * \code
 *
 * #include <stfy/hal.hpp>
 * #include <stfy/Sys.hpp>
 *
 *  //This will be executed when the event is triggered
 * int event_callback(void * args, const void * data){
 * 	volatile int * done = (int*)args;
 * 	*done = 1;
 * 	return 0; //return 0 to clear the callback and 1 to keep it
 * }
 *
 * void eint_example(){
 * 	volatile int done;
 * 	Eint eint(0); //use eint port 0 (pin 2.10 on lpc17xx devices)
 *
 * 	eint.init(); //open and set attributes
 *
 * 	eint.set_action(0, //use channel 0
 * 		EINT_ACTION_EVENT_FALLING, //trigger on a falling edge
 * 		event_callback, //execute this function (in privileged mode when complete)
 * 		&done //pass this to event_callback as the first argumen
 * 		);
 *
 * 	while( done == 0 ){
 * 		Timer::wait_msec(1); //wait for the signal to arrive
 * 	}
 *
 * }
 *
 *  //alternatively you can configure (set_action() and block by writing hwpl_action_t to the device)
 *
 *  hwpl_action_t action;
 *  action.channel = 0;
 *  action.event = EINT_ACTION_EVENT_FALLING;
 *  action.callback = 0; //this is ignored and doesn't need to be set
 *  action.context = 0; //this is ignored and doesn't need to be set
 *  eint.write(&action, sizeof(action)); //this will block until a falling edge arrives
 *
 * \endcode
 *
 */
class Eint : public Periph {
public:
	Eint(port_t port);
	/*! \details Get the attributes for the external interrupt. */
	int get_attr(eint_attr_t * attr);
	/*! \details Set the attributes for the external interrupt. */
	int set_attr(const eint_attr_t * attr);
	/*! \details Set the action associated with the external interrupt. */
	int set_action(const eint_action_t & action);

	/*! \details Read the value of the pin */
	bool get_value(){
		eint_attr_t a;
		get_attr(&a);
		return a.value != 0;
	}

	/*! \details Set the action using the specified parameters */
	int set_action(int channel, int event, mcu_callback_t callback, void * context){
		eint_action_t action;
		action.channel = channel;
		action.event = event;
		action.callback = callback;
		action.context = context;
		return set_action(action);
	}

	/*! \details Set the attributes using the specified pin assignment. */
	int set_attr(uint8_t pin_assign = 0, uint16_t mode = Pin::INPUT | Pin::PULLUP){
		eint_attr_t attr;
		attr.pin_assign = pin_assign;
		attr.mode = mode;
		return set_attr(&attr);
	}

	/*! \details Open and set attributes using specified pin assignment. */
	int init(uint8_t pin_assign = 0, uint16_t mode = Pin::INPUT | Pin::PULLUP){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(pin_assign, mode);
	}

	/*! \details Events used with set_action() */
	enum {
		 EVENT_UNCONFIGURED /*! \brief Unconfigured */ = EINT_ACTION_EVENT_UNCONFIGURED,
		 EVENT_RISING /*! \brief Event on rising edge */ = EINT_ACTION_EVENT_RISING,
		 EVENT_FALLING /*! \brief Event on falling edge */ = EINT_ACTION_EVENT_FALLING,
		 EVENT_BOTH /*! \brief Event on both edges */ = EINT_ACTION_EVENT_BOTH,
		 EVENT_LOW /*! \brief Event when low */ = EINT_ACTION_EVENT_LOW,
		 EVENT_HIGH /*! \brief Event when high */ = EINT_ACTION_EVENT_HIGH
	};

#ifdef __MCU_ONLY__
	int close();
#endif

private:

};

};

#endif /* STFY_APP_EINT_HPP_ */
