/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef PWM_HPP_
#define PWM_HPP_

#include <iface/dev/pwm.h>
#include "Periph.hpp"

namespace Hal {

/*! \brief PWM Class
 * \details This class implements PWM outputs.
 *
 * Here is an example of how to use the Pwm class
 *
 * \code
 * #include <stfy/Hal.hpp>
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
class Pwm : public Periph {
public:
	Pwm(port_t port);
	/*! \details Get PWM attributes */
	int get_attr(pwm_attr_t * attr);
	/*! \details Set PWM attributes */
	int set_attr(const pwm_attr_t * attr);
	/*! \details Set PWM duty cycle */
	int set_duty_cycle(const pwm_reqattr_t * req);

#ifdef __MCU_ONLY__
	using Pblock::write;
	int write(const void * buf, int nbyte);
#endif

	/*! \details Set the PWM duty cycle (specify individual values) */
	int set_duty_cycle(uint8_t channel, pwm_duty_t duty){
		pwm_reqattr_t req;
		req.channel = channel;
		req.duty = duty;
		return set_duty_cycle(&req);
	}

	/*! \details Set PWM attributes (specify individual values) */
	int set_attr(uint8_t enabled_channels,
			uint32_t freq = 1000000,
			pwm_duty_t top = 1000,
			uint16_t flags = 0,
			uint8_t pin_assign = 0){
		pwm_attr_t attr;
		attr.enabled_channels = enabled_channels;
		attr.freq = freq;
		attr.top = top;
		attr.pin_assign = pin_assign;
		attr.flags = flags;
		return set_attr(&attr);
	}

	/*! \details Open PWM and set attributes as specified */
	int init(uint8_t enabled_channels,
			uint32_t freq = 1000000,
			pwm_duty_t top = 1000,
			uint16_t flags = 0,
			uint8_t pin_assign = 0){
		if( open() < 0 ){
			return -1;
		}

		return set_attr(enabled_channels, freq, top, flags, pin_assign);
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
