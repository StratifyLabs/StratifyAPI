/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_ADC_HPP_
#define SAPI_ADC_HPP_

#include <sos/dev/adc.h>
#include "Periph.hpp"

namespace hal {


/*! \brief ADC Peripheral Class
 * \details This class implements ADC device peripherals.
 *
 * \code
 * #include <stfy/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Adc adc(0);  //create an instance of ADC to access port 0
 * 	adc_sample_t samples[16];
 * 	adc.init((1<<0)|(1<<1)); //open() and set_attr() enabling channels 0 and 1
 * 	adc.read(0, samples, 16*sizeof(adc_sample_t)); //read 16 samples from channel 0
 * 	adc.read(1, samples, 16*sizeof(adc_sample_t)); //read 16 samples from channel 1
 * 	adc.close(); //close the ADC
 * }
 * \endcode
 *
 */
class Adc : public Periph<adc_info_t, adc_attr_t, 'a'> {
public:
	/*! \details Initializes the object with \a port. */
	Adc(port_t port);

	enum {
		FLAG_LEFT_JUSTIFIED = ADC_FLAG_LEFT_JUSTIFIED,
		FLAG_RIGHT_JUSTIFIED = ADC_FLAG_RIGHT_JUSTIFIED
	};


	int set_attr(u32 o_flags, mcu_pin_t channel0, u32 freq, mcu_pin_t channel1) const {
		adc_attr_t attr;
		attr.pin_assignment[0] = channel0;
		attr.pin_assignment[1] = channel1;
		attr.pin_assignment[2] = mcu_invalid_pin();
		attr.pin_assignment[3] = mcu_invalid_pin();
		attr.o_flags = o_flags;
		attr.freq = freq;
		return Periph::set_attr(attr);
	}


	/*! \details Opens the ADC port and sets the attributes as specified.
	 *
	 * @param enabled_channels Enabled channels as a bitmask
	 * @param freq The ADC clock frequency
	 * @param pin_assign The pin assignment value
	 * @return Zero on success
	 *
	 */
	int init(u32 o_flags, mcu_pin_t channel0, u32 freq, mcu_pin_t channel1){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, channel0, freq, channel1);
	}


#ifdef __MCU_ONLY__
	using Pblock::read;
	int read(void * buf, int nbyte);
	int close();
#endif


private:

};

};

#endif /* SAPI_ADC_HPP_ */
