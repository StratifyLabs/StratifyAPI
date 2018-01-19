/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_ADC_HPP_
#define SAPI_ADC_HPP_

#include <sos/dev/adc.h>
#include "PinAssignment.hpp"
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
 * 	u32 samples[16];
 * 	adc.init(); //initialize the ADC using the default configuration
 * 	adc.read(0, samples, 16*sizeof(u32)); //read 16 samples from channel 0
 * 	adc.read(1, samples, 16*sizeof(u32)); //read 16 samples from channel 1
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
		FLAG_SET_CONVERTER /*! Set to configure the ADC */ = ADC_FLAG_SET_CONVERTER,
		FLAG_IS_LEFT_JUSTIFIED /*! Set to specify left justified data */ = ADC_FLAG_IS_LEFT_JUSTIFIED,
		FLAG_IS_RIGHT_JUSTIFIED /*! Set to specify right justified data */ = ADC_FLAG_IS_RIGHT_JUSTIFIED,
	};

	/*! \details Sets the attributes of the ADC.
	 *
	 * @param o_flags The ADC flags (e.g. FLAG_SET_CONVERTER)
	 * @param freq The ADC frequency (set to 0 to use the maximum available)
	 * @param pin_assignment A pointer to the pin assignment data structure (0 to use default pin assignment)
	 * @return Zero on success or less than zero for an error
	 */
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
	 * @param o_flags The ADC flags (e.g. FLAG_SET_CONVERTER)
	 * @param freq The ADC frequency (set to 0 to use the maximum available)
	 * @param pin_assignment A pointer to the pin assignment data structure (0 to use default pin assignment)
	 * @return Zero on success or less than zero for an error
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
