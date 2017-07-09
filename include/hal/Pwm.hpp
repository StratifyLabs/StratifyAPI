/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef PWM_HPP_
#define PWM_HPP_

#include <sos/dev/pwm.h>
#include "Periph.hpp"

namespace hal {

/*! \brief PWM Class
 * \details This class implements PWM outputs.
 *
 * Here is an example of how to use the Pwm class
 *
 * \code
 * #include <stfy/hal.hpp>
 *
 *
 * Pwm pwm(1); //use PWM port 1
 *
 * pwm.init(1<<0|1<<1,
 * 	1000000, //PWM frequency
 * 	1000, //Top value at 1MHz the PWM period is 1ms
 * 	0, //PWM flags (not used on lpc17xx)
 * 	__PWM1_PINASSIGN_PORT2_0_1_2_3_4_5); //PWM pin assignment (determines how channels map to pins)
 *
 * 	pwm.set(0, 500); //set channel 0 to 50% duty cycle (500/1000)
 * 	pwm.set(1, 250); //set channel 1 to 25% duty cycle
 *
 * \endcode
 *
 *
 */
class Pwm : public Periph<pwm_info_t, pwm_attr_t, 'p'> {
public:
	Pwm(port_t port);

	enum {
		FLAG_IS_ACTIVE_HIGH = PWM_FLAG_IS_ACTIVE_HIGH,
		FLAG_IS_ACTIVE_LOW = PWM_FLAG_IS_ACTIVE_LOW
	}

	int set_channel(u32 loc, u32 value){
		return set_channel(loc, value, I_PWM_SET);
	}

	int set_channel(const mcu_channel_t & channel){
		return set_channel(channel, I_PWM_SET);
	}


	int set_attr(u32 o_flags, mcu_pin_t channel0, u32 freq, u32 top, mcu_pin_t channel1 = {0xff, 0xff}){
		pwm_attr_t attr;
		attr.o_flags = o_flags;
		memset(attr.pin_assignment, 0xff, PWM_PIN_ASSIGNMENT_COUNT*sizeof(mcu_pin_t));
		attr.pin_assignment[0] = channel0;
		attr.freq = freq;
		attr.top = top;
	}

	int init(u32 o_flags, mcu_pin_t channel0, u32 freq, u32 top, mcu_pin_t channel1 = {0xff, 0xff}){

		if( open() < 0 ){
			return -1;
		}

		return set_attr(o_flags, channel0, freq, top, channel1);
	}

#ifdef __MCU_ONLY__
	//using Pblock::write;
	//int write(const void * buf, int nbyte);
	int close();
#endif


private:

};

};

#endif /* PWM_HPP_ */
