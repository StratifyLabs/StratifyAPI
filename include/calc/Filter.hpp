/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_CALC_FILTER_HPP_
#define SAPI_CALC_FILTER_HPP_

#include <cstdio>
#include "../api/CalcObject.hpp"

namespace calc {

template<typename T, typename C> class SimpleFilter {
public:
	SimpleFilter(){}

	/*! \details Calculates the next value
	 * and returns a reference to this object.
	 *
	 */
	C & operator << (T a){
		calculate(a);
		return (C&)*this;
	}


	/*! \details Calculates the next value based on the given input
	 * and returns the new average value.
	 *
	 */
	virtual T calculate(T a) = 0;

	/*! \details Returns a copy of the present value. */
	T present_value() const { return m_present_value; }

	/*! \details Sets the present value of the filter. */
	void set_present_value(const T & value){ m_present_value = value; }

protected:
	T m_present_value;
};

/*! \brief Macro for creating the alpha value for 32-bit calculations */
/*! \details This value can be set between 0.0 and 1.0.  More weight is
 * given to older samples when the valeu is close to 0.0.  When the value is set
 * to 1.0, no averaging occurs.
 */
#define LOW_PASS_FILTER_S32_ALPHA(x) ( (u16)(x * 65535) )

/*! \brief Macro for creating the alpha avelu for 16-bit calculations */
/*! \details See \ref LOW_PASS_FILTER_S32_ALPHA for details */
#define LOW_PASS_FILTER_S16_ALPHA(x) ( (u8)(x * 255) )

/*! \brief LowPassFilter template class */
/*! \details This class is a template for a simple low pass filter
 *	(also known as an exponential moving average) calculation.
 *
 * The filter uses the following difference equation:
 *
 * $$ y[n] = \alpha * x[n] + (1 - \alpha) * y[n] $$
 *
 * Alpha is a value between 0 and 1. If alpha is 0, there is no
 * filtering. If alpha is close to 1, the cutoff frequency is closer
 * to zero. The alpha value and the sampling frequency both
 * affect the cutoff frequency.
 *
 * There are several classes that populate the template with
 * reasonable values:
 *
 * - LowPassFilterS32
 * - LowPassFilterS16
 * - LowPassFilterU32
 * - LowPassFilterU16
 *
 * LowPassFilterF32 is the same filter but with floating point values.
 *
 * The following is an example of using the Ema class using 32-bit integers.
 * ```
 * //md2code:include
 * #include <sapi/hal.hpp>
 * #include <sapi/calc.hpp>
 * ```
 *
 * ```
 * //md2code:main
 *	//first initialize a filter with a value of 0 and with an averaging constant of 0.1
 * LowPassFilterS32 filter(0, LOW_PASS_FILTER_S32_ALPHA(0.1));
 *	//Or do:  LowPassFilter<u16, s32, s64> filter(0, LOW_PASS_FILTER_S32_ALPHA(0.1));
 *
 *	//now use the ADC to get some data
 * Adc adc(0);
 * u16 sample;
 * adc.initialize();
 *
 * for(u32 i=0; i < 100; i++){  //take 100 samples and filter as we go
 * 	adc.read(
 *       arg::Location(0),
 *       arg::DestinationBuffer(&sample),
 *       arg::Size(sizeof(sample))
 *    );
 * 	filter << sample;
 * 	printf("%ld %d %ld;\n", i, sample, filter.present_value());
 * }
 * ```
 *
 * For more information on this filter see <a href="http://stratifylabs.co/embedded%20design%20tips/2013/10/04/Tips-An-Easy-to-Use-Digital-Filter">this wiki article</a>.
 *
 */
template<typename intsmall, typename intmedium, typename intlarge>class LowPassFilter : public SimpleFilter<intmedium, LowPassFilter<intsmall, intmedium, intlarge>> {
public:
	/*! \details Constructs a new Ema object.
	 *
	 * @param start Initial value
	 * @param alpha Averaging value
	 */
	LowPassFilter(intmedium start, intsmall alpha){ m_average = start;  this->m_alpha = alpha; }
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

/*! \brief LowPassFilter class (s32) */
/*! \details See \ref LowPassFilter for details */
class LowPassFilterS32 : public LowPassFilter<u16, s32, s64> {
public:
	/*! \details Construct a EMA object for a signed 32 bit calculations */
	LowPassFilterS32(s32 start, u16 alpha) : LowPassFilter(start, alpha){}
};

/*! \brief LowPassFilter class (s16) */
/*! \details See \ref LowPassFilter for details */
class LowPassFilterS16 : public LowPassFilter<u8, s16, s32> {
public:
	/*! \details Construct a EMA object for a signed 16 bit calculations */
	LowPassFilterS16(s16 start, u8 alpha) : LowPassFilter(start, alpha){}
};

/*! \brief LowPassFilter class (u32) */
/*! \details See \ref LowPassFilter for details */
class LowPassFilterU32 : public LowPassFilter<u16, u32, u64> {
public:
	/*! \details Construct a EMA object for a unsigned 32 bit calculations */
	LowPassFilterU32(u32 start, u16 alpha) : LowPassFilter(start, alpha){}
};

/*! \brief LowPassFilter class (u16) */
/*! \details See \ref LowPassFilter for details */
class LowPassFilterU16 : public LowPassFilter<u8, u16, u32> {
public:
	/*! \details Construct a EMA object for a unsigned 16 bit calculations */
	LowPassFilterU16(u16 start, u8 alpha) : LowPassFilter(start, alpha){}
};

/*! \brief LowPassFilterF32 class (float) */
/*! \details See \ref LowPassFilter for details */
class LowPassFilterF32 : public SimpleFilter<float, LowPassFilterF32> {
public:
	/*! \details Constructs a EMA object for floating point calculations */
	LowPassFilterF32(float start = 0.0f, float alpha = 1.0f);

	/*! \details Sets the filtering value.
	 *
	 * @param value The alpha value for the filter (0.0f to 1.0f)
	 *
	 * When alpha is 1.0f, no filtering is applied. The closer
	 * alpha is to 0.0f, the lower the cutoff frequency is for the filter.
	 *
	 */
	void set_alpha(float value){
		m_alpha = value;
	}


	/*! \details Calculates the alpha value for the given
	 * magnitude (impulse response) and sampling frequency.
	 *
	 */
	float calculate_alpha(float sampling_frequency, float magnitude = 0.5f);

	void reset(float start);
	float calculate(float in);

private:
	float m_alpha;
};


/*! \brief HighPassFilterF32 class (float) */
/*! \details This class implements a simple high
 * pass filter. The filter is defined by the following difference
 * equation:
 *
 * $$ y[n] = x[n] - r \cdot x[n-1] + y[n-1] $$
 *
 * The *r* value should be between zero and one. Values closer
 * to one have a lower cutoff frequency.
 *
 * ```
 * #include <sapi/calc.hpp>
 *
 * HighPassFilterF32 filter(0.0f, 0.9f);
 *
 * filter << 0.1f; //input 0.1 into the filter
 * filter << 0.9f; //input another value
 *
 * printf("Filter value is %0.1f\n", filter.present_value());
 * ```
 *
 */
class HighPassFilterF32 : public SimpleFilter<float, HighPassFilterF32> {
public:
	/*! \details Constructs a High pass filter object for floating point calculations. */
	HighPassFilterF32(float start, float r_value);

	/*! \details Resets the filter to the given start value. */
	void reset(float start);

	/*! \details Updates the r value of the filter. */
	void set_r_value(float r_value);

	void set_initial_input(float value){
		m_last_input = value;
	}

	float calculate(float input);

private:
	float m_last_input;
	float m_r_value;
};



}
#endif /* SAPI_CALC_FILTER_HPP_ */
