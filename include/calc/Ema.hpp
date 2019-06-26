/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CALC_EMA_HPP_
#define SAPI_CALC_EMA_HPP_

#include <cstdio>
#include "../api/CalcObject.hpp"

namespace calc {

/*! \cond */

/*! \brief Macro for creating the alpha value for 32-bit calculations */
/*! \details This value can be set between 0.0 and 1.0.  More weight is
 * given to older samples when the valeu is close to 0.0.  When the value is set
 * to 1.0, no averaging occurs.
 */
#define DSP_EMA_I32_ALPHA(x) ( (u16)(x * 65535) )

/*! \brief Macro for creating the alpha avelu for 16-bit calculations */
/*! \details See \ref DSP_EMA_I32_ALPHA for details */
#define DSP_EMA_I16_ALPHA(x) ( (u8)(x * 255) )

/*! \brief Exponential Moving Average template class */
/*! \details This class is a template for an exponential moving average (EMA)
 * calculation.  The EMA is a type of low pass filter and is helpful when trying
 * to smooth out data that is sampled on the ADC.
 *
 * The following is an example of using the Ema class using 32-bit integers.
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/calc.hpp>
 * #include <cstdio>
 *
 *  //first initialize a filter with a value of 0 and with an averaging constant of 0.1
 * Ema_s32 filter(0, DSP_EMA_I32_ALPHA(0.1));
 *  //Or do:  Ema<u16, s32, s64> filter(0, DSP_EMA_I32_ALPHA(0.1));
 *
 *  //now use the ADC to get some data
 * Adc adc(0);
 * adc_sample_t sample;
 * adc.init(1<<0); //initialize with channel 0 enabled
 *
 * for(i=0; i < 100; i++){  //take 100 samples and filter as we go
 * 	adc.read(0, &sample, sizeof(sample));
 * 	filter.calc(sample);
 * 	printf("%d %d %d;\n", i, sample, filter.avg());
 * }
 * \endcode
 *
 * The filter is easy-to-use and uses minimal resources.  For more information on
 * EMA filtering see <a href="http://stratifylabs.co/embedded%20design%20tips/2013/10/04/Tips-An-Easy-to-Use-Digital-Filter">this wiki article</a>.
 *
 */
template<typename intsmall, typename intmedium, typename intlarge>class Ema : public api::CalcWorkObject {
public:
	/*! \details Constructs a new Ema object.
	 *
	 * @param start Initial value
	 * @param alpha Averaging value
	 */
	Ema(intmedium start, intsmall alpha){ m_average = start;  this->m_alpha = alpha; }
	static intmedium small_max(){ return 1<<(8*sizeof(intsmall)); }

	/*! \details Calculates the next average using an input value.
	 *
	 * @param in Input value
	 * @return The updated average (same as average())
	 */
	intmedium calculate(intmedium in){
		intlarge tmp0;
		tmp0 = (intlarge)in * (m_alpha) + (intlarge)m_average * (small_max() - m_alpha);
		m_average = (intmedium)(((intlarge)tmp0 + (intlarge)small_max()) >> (sizeof(intsmall)*8));
		return m_average;
	}

	intmedium calc(intmedium in){ return calculate(in); }

	/*! \details Accesses the current average (no calculations are made here).
	 *
	 * @return The current average value
	 */
	intmedium present_value() const { return m_average; }
	intmedium average() const { return m_average; }

	/*! \details Sets the average value.
	 *
	 * @param v The new initial value
	 *
	 * This method can also be used to set the initial value.
	 */
	void set_average(intmedium v){ m_average = v; }

private:
	intmedium m_average;
	intsmall m_alpha;
};

/*! \brief Exponential Moving Average class (s32) */
/*! \details See \ref Ema for details */
class Ema_s32 : public Ema<u16, s32, s64> {
public:
	/*! \details Construct a EMA object for a signed 32 bit calculations */
	Ema_s32(s32 start, u16 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (s16) */
/*! \details See \ref Ema for details */
class Ema_s16 : public Ema<u8, s16, s32> {
public:
	/*! \details Construct a EMA object for a signed 16 bit calculations */
	Ema_s16(s16 start, u8 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (u32) */
/*! \details See \ref Ema for details */
class Ema_u32 : public Ema<u16, u32, u64> {
public:
	/*! \details Construct a EMA object for a unsigned 32 bit calculations */
	Ema_u32(u32 start, u16 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (u16) */
/*! \details See \ref Ema for details */
class Ema_u16 : public Ema<u8, u16, u32> {
public:
	/*! \details Construct a EMA object for a unsigned 16 bit calculations */
	Ema_u16(u16 start, u8 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (float) */
/*! \details See \ref Ema for details */
class Ema_f : public api::CalcWorkObject {
public:
	/*! \details Constructs a EMA object for floating point calculations */
	Ema_f(float start, float alpha){ m_alpha = alpha; m_average = start; }
	static float small_max(){ return 1.0f; }
	/*! \details Calculates the next value based on the given input */
	float calculate(float in){
		float tmp;
		m_average = in * (m_alpha) + m_average * (1.0f - m_alpha);
		return m_average;
	}

	float calc(float in){ return calculate(in); }

	/*! \details Accesses the present value of the filter. */
	float present_value() const { return m_average; }

	float average() const { return m_average; }
	/*! \details Sets the present value of the filter. */
	void set_present_value(float v){ m_average = v; }
	void set_average(float v){ m_average = v; }

	//deprecated methods
	void set(float v){ m_average = v; }
	float avg() const { return m_average; }

private:
	float m_average;
	float m_alpha;
};


//deprecated
typedef Ema_f Ema_float;

/*! \endcond */

}
#endif /* SAPI_CALC_EMA_HPP_ */
