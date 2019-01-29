/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_ADC_HPP_
#define SAPI_ADC_HPP_

#include <sos/dev/adc.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"

namespace hal {

/*! \brief ADC Info Class
 *
 *
 */
class AdcInfo : public api::HalInfoObject {
public:
	AdcInfo(){ memset(&m_adc_info, 0, sizeof(m_adc_info)); }

	bool is_valid() const { return m_adc_info.bytes_per_sample; }

	u32 o_flags() const { return m_adc_info.o_flags; }
	u32 o_events() const { return m_adc_info.o_events; }
	u32 resolution() const { return m_adc_info.resolution; }
	u8 bytes_per_sample() const { return m_adc_info.bytes_per_sample; }
	u32 frequency() const { return m_adc_info.freq; }
	u32 maximum_value() const { return m_adc_info.maximum; }
	u32 reference_millivolts() const { return m_adc_info.reference_mv; }


	u32 calculate_millivolts(u32 sample_value) const {
		return (sample_value * reference_millivolts() + maximum_value()/2) / maximum_value();
	}


	const adc_info_t & info() const{ return m_adc_info; }
	adc_info_t & info(){ return m_adc_info; }

private:
	adc_info_t m_adc_info;
};


/*! \brief ADC Attributes Class
 * \details This class is for containing ADC attributes.
 */
class AdcAttributes : public PinAssignmentPeriphAttributes<adc_attr_t, adc_pin_assignment_t> {
public:

	AdcAttributes(){}

	AdcAttributes(u32 o_flags, u32 freq){
		set_flags(o_flags);
		set_freq(freq);
	}

	void set_channel(u16 channel){ m_attr.channel = channel; }
	void set_rank(u32 rank){ m_attr.rank = rank; }
	void set_sampling_time(u32 sampling_time){ m_attr.sampling_time = sampling_time; }

	/*! \details Configures a channel in a group for sampling.
	 *
	 * @param channel The channel number to sample
	 * @param rank The rank (order) of the channel
	 * @param sampling_time The sampling time (in ADC clocks for the channel)
	 *
	 * \code
	 * AdcAttributes adc_attributes;
	 * adc_attributes.configure_group_channel(0, 1, 15);
	 * Adc adc(0);
	 *
	 * adc.init();
	 * adc.set_attributes(adc_attributes);
	 * \endcode
	 *
	 */
	void configure_group_channel(u16 channel, u32 rank, u32 sampling_time = 15){
		set_flags(ADC_FLAG_SET_CHANNELS | ADC_FLAG_IS_GROUP);
		set_channel(channel);
		set_rank(rank);
		set_sampling_time(sampling_time);
	}

};

typedef AdcAttributes AdcAttr;

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
class Adc : public Periph<adc_info_t, adc_attr_t, AdcAttributes, ADC_IOC_IDENT_CHAR> {
public:

	/*! \details Initializes the object with \a port. */
	Adc(port_t port);

	enum {
		FLAG_SET_CONVERTER = ADC_FLAG_SET_CONVERTER,
		FLAG_IS_LEFT_JUSTIFIED = ADC_FLAG_IS_LEFT_JUSTIFIED,
		FLAG_IS_RIGHT_JUSTIFIED = ADC_FLAG_IS_RIGHT_JUSTIFIED,

		SET_CONVERTER /*! See ADC_FLAG_SET_CONVERTER */ = ADC_FLAG_SET_CONVERTER,
		IS_LEFT_JUSTIFIED /*! See ADC_FLAG_IS_LEFT_JUSTIFIED */ = ADC_FLAG_IS_LEFT_JUSTIFIED,
		IS_RIGHT_JUSTIFIED /*! Set to specify right justified data */ = ADC_FLAG_IS_RIGHT_JUSTIFIED,
		SET_MASTER = ADC_FLAG_SET_MASTER /*! Used with MCUs that have more than one ADC that can operate in master/slave mode */,
		SET_SLAVE = ADC_FLAG_SET_SLAVE /*! Used with MCUs that have more than one ADC that can operate in master/slave mode */,
		IS_TRIGGER_TMR = ADC_FLAG_IS_TRIGGER_TMR /*! Used to trigger the ADC read on a timer event */,
		IS_TRIGGER_EINT = ADC_FLAG_IS_TRIGGER_EINT /*! Used to trigger the ADC read on a external interrupt */,
		SET_CHANNELS = ADC_FLAG_SET_CHANNELS /*! Configure the channels withouth changing ADC settings */,
		IS_SCAN_MODE = ADC_FLAG_IS_SCAN_MODE /*! ADC will read every enabled channel when reading rather than the channel based on the location value */,
		IS_TRIGGER_EINT_EDGE_RISING /*! Trigger the sample on the rising edge */ = ADC_FLAG_IS_TRIGGER_EINT_EDGE_RISING,
		IS_TRIGGER_EINT_EDGE_FALLING /*! Trigger the sample on the falling edge */ = ADC_FLAG_IS_TRIGGER_EINT_EDGE_FALLING,
		IS_GROUP /*! Set channel as part of a group */ = ADC_FLAG_IS_GROUP,
		IS_CONTINOUS_CONVERSION /*! Start the next conversion as soon as the previous conversion completes */ = ADC_FLAG_IS_CONTINOUS_CONVERSION

	};

	/*! \details Returns an AdcInfo object associated with the ADC.
	 *
	 */
	AdcInfo get_info() const;

	int set_attr(u32 o_flags, u32 freq, const adc_pin_assignment_t * pin_assignment = 0) const {
		adc_attr_t attr;
		attr.o_flags = o_flags;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(adc_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(adc_pin_assignment_t));
		}
		attr.freq = freq;
		return set_attributes(attr);
	}

	int init(u32 o_flags, u32 freq, const adc_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, pin_assignment);
	}

private:

};

}

#endif /* SAPI_ADC_HPP_ */
