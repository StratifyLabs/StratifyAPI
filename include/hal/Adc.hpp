/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_ADC_HPP_
#define SAPI_ADC_HPP_

#include <sos/dev/adc.h>
#include "Periph.hpp"

namespace hal {

/*! \brief ADC Pin Assignment
 * \details This class allows simple manipulation of the adc_pin_assignment_t.
 */
class AdcPinAssignment : public PinAssignment<adc_pin_assignment_t>{};


/*! \brief ADC Peripheral Class
 * \details This class implements ADC device peripherals.
 *
 * \code
 * #include <sapi/hal.hpp>
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
		FLAG_LEFT_JUSTIFIED = ADC_FLAG_IS_LEFT_JUSTIFIED,
		FLAG_RIGHT_JUSTIFIED = ADC_FLAG_IS_RIGHT_JUSTIFIED,
	};

	int set_attr(u32 o_flags, u32 freq, const adc_pin_assignment_t * pin_assignment = 0) const {
		adc_attr_t attr;
		attr.o_flags = o_flags;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(adc_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(adc_pin_assignment_t));
		}
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
	int init(u32 o_flags, u32 freq, const adc_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, pin_assignment);
	}

	using Periph::init;
	using Periph::set_attr;


#ifdef __MCU_ONLY__
	using Pblock::read;
	int read(void * buf, int nbyte);
	int close();
#endif


private:

};

};

#endif /* SAPI_ADC_HPP_ */
