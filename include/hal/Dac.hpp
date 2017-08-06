/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DAC_HPP_
#define DAC_HPP_

#include <sos/dev/dac.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"

namespace hal {

/*! \brief DAC Pin Assignment
 * \details This class allows simple manipulation of the dac_pin_assignment_t.
 */
class DacPinAssignment : public PinAssignment<dac_pin_assignment_t>{};

/*! \brief DAC Class
 * \details DAC (Digital to Analog Converter) Class
 *
 * The DAC class is used to write data to the digital to analog converter.  Here is an example:
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/sys.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Dac dac(0);
 * 	u32 samples[50];
 * 	dac.init(); //init with default settings
 *
 *  //here, write meaningful values to samples
 *
 * 	dac.write(0, samples, 50*sizeof(u32)); //This returns when all 50 samples are written
 *
 *  //if you want to do other things while the sample is written use AIO
 * 	Aio aio(samples, 50*sizeof(u32));
 *
 * 	dac.write(aio);
 *  //Here you can do other things while the data is written
 * 	while( aio.is_busy() == true ){
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

	enum {
		FLAG_SET_CONVERTER /*! Set to configure the converter */ = DAC_FLAG_SET_CONVERTER,
		FLAG_LEFT_JUSTIFIED /*! Set to left justify the data */ = DAC_FLAG_IS_LEFT_JUSTIFIED,
		FLAG_RIGHT_JUSTIFIED /*! Set to right justify the data */ = DAC_FLAG_IS_RIGHT_JUSTIFIED,
	};

	/*! \details Sets the DAC attributes using specified values.
	 *
	 * @param o_flags Enabled channels as a bitmask
	 * @param freq DAC output frequency
	 * @param pin_assignment The pins to use (if null, default pins will be used if available)
	 *
	 */
	int set_attr(u32 o_flags, u32 freq, const dac_pin_assignment_t * pin_assignment = 0) const {
		dac_attr_t attr;
		attr.o_flags = o_flags;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(dac_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(dac_pin_assignment_t));
		}
		attr.freq = freq;
		return set_attr(attr);
	}


	/*! \details Opens the DAC and sets the attributes using specified values.
	 *
	 * See set_attr() for parameter descriptions.
	 *
	 */
	int init(u32 o_flags, u32 freq, const dac_pin_assignment_t * pin_assignment = 0){

		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, pin_assignment);
	}

	/*! \details Sets the value of the DAC channel.
	 *
	 * @param value Value to write to the DAC
	 * @param channel The DAC channel
	 * @return Zero on success
	 */
	int set_channel(u32 loc, u32 value) const {
		return Periph::set_channel(loc, value, I_DAC_SET);
	}

	/*! \details Gets the current value of the DAC channel.
	 *
	 * @param loc The DAC channel to get
	 * @return The current value of the channel or (u32)-1 if ther is an error
	 */
	u32 get_channel(u32 loc) const {
		return Periph::get_channel(loc, I_DAC_GET);
	}

	using Periph::init;
	using Periph::set_attr;

private:



};

};

#endif /* DAC_HPP_ */
