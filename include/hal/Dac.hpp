/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DAC_HPP_
#define DAC_HPP_

#include <sos/dev/dac.h>
#include "Periph.hpp"

namespace hal {

/*! \brief DAC Class
 * \details DAC (Digital to Analog Converter) Class
 *
 * The DAC class is used to write data to the digital to analog converter.  Here is an example:
 *
 * \code
 * #include <stfy/hal.hpp>
 * #include <stfy/Sys.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Dac dac(0);
 * 	dac_sample_t samples[50];
 * 	dac.init(1<<0, 50000); //enable channel 0 at 50KHz
 *
 *  //here, write meaningful values to samples
 *
 * 	dac.write(0, samples, 50*sizeof(dac_sample_t)); //This returns when all 50 samples are written
 *
 *  //if you want to do other things while the sample is written use AIO
 * 	Aio aio(samples, 50*sizeof(dac_sample_t));
 *
 * 	dac.write(aio);
 *  //Here you can do other things while the data is written
 * 	while( aio.inprogress() == true ){
 * 		Timer::wait_usec(500); //wait until the write is complete
 * 	}
 *
 * 	dac.close(); //this will power off the DAC if nothing else has it open
 *
 * 	return 0;
 * }
 *
 * \endcode
 *
 */
class Dac : public Periph<dac_info_t, dac_attr_t, 'd'> {
public:
	Dac(port_t port);

	using Periph::set_attr;

	/*! \details Sets the DAC attributes using specified values.
	 *
	 * @param enabled_channels Enabled channels as a bitmask
	 * @param freq DAC output frequency
	 * @param pin_assign The output pin assignment
	 *
	 */
	int set_attr(u32 o_flags, mcu_pin_t channel0, u32 freq, mcu_pin_t channel1) const {
		dac_attr_t attr;
		memset(attr.pin_assignment, 0xff, DAC_PIN_ASSIGNMENT_COUNT*sizeof(mcu_pin_t));
		attr.o_flags = o_flags;
		attr.pin_assignment[0] = channel0;
		attr.pin_assignment[1] = channel1;
		attr.freq = freq;
		return set_attr(attr);
	}


	/*! \details Opens the DAC and sets the attributes using specified values.
	 *
	 * @param enabled_channels Enabled channels as a bitmask
	 * @param freq DAC output frequency
	 * @param pin_assign The output pin assignment
	 *
	 */
	int init(u32 o_flags, mcu_pin_t channel0, u32 freq, mcu_pin_t channel1){

		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, channel0, freq, channel1);
	}

	/*! \details Sets the value of the DAC.
	 *
	 * @param value Value to write to the DAC
	 * @param channel The DAC channel
	 * @return Zero on success
	 */
	int set_channel(u32 loc, u32 value) const {
		return Periph::set_channel(loc, value, I_DAC_SET);
	}

	u32 get_channel(u32 loc) const {
		return Periph::get_channel(loc, I_DAC_GET);
	}


private:



};

};

#endif /* DAC_HPP_ */
