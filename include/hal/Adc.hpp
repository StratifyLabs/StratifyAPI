/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_ADC_HPP_
#define STFY_APP_ADC_HPP_

#include <iface/dev/adc.h>
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
class Adc : public Periph {
public:
	/*! \details Initializes the object with \a port. */
	Adc(port_t port);

	/*! \details Gets the ADC attributes */
	int get_attr(adc_attr_t & attr);
	/*! \details Sets the ADC attributes */
	int set_attr(const adc_attr_t & attr);

	/*! \details Sets the ADC attributes.
	 *
	 * @param enabled_channels Enabled channels as a bitmask
	 * @param freq The ADC clock frequency
	 * @param pin_assign The pin assignment value
	 * @return Zero on success
	 *
	 */
	int set_attr(u16 enabled_channels, u32 freq = ADC_MAX_FREQ, u8 pin_assign = 0){
		adc_attr_t attr;
		attr.enabled_channels = enabled_channels;
		attr.freq = freq;
		attr.pin_assign = pin_assign;
		return set_attr(attr);
	}

	/*! \details Opens the ADC port and sets the attributes as specified.
	 *
	 * @param enabled_channels Enabled channels as a bitmask
	 * @param freq The ADC clock frequency
	 * @param pin_assign The pin assignment value
	 * @return Zero on success
	 *
	 */
	int init(u16 enabled_channels, u32 freq = ADC_MAX_FREQ, u8 pin_assign = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(enabled_channels, freq, pin_assign);
	}


#ifdef __MCU_ONLY__
	using Pblock::read;
	int read(void * buf, int nbyte);
	int close();
#endif


private:

};

};

#endif /* STFY_APP_ADC_HPP_ */
