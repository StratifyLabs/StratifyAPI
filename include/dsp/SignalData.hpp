#ifndef SAPI_DSP_SIGNAL_DATA_HPP_
#define SAPI_DSP_SIGNAL_DATA_HPP_

#include "../api/DspObject.hpp"
#include "../var/Vector.hpp"

namespace dsp {

class FftComplexQ15;
class FftComplexQ31;
class FftComplexF32;
class FftRealQ15;
class FftRealQ31;
class FftRealF32;
class FirFilterQ15;
class FirFilterQ31;
class FirFilterF32;
class BiquadFilterQ15;
class BiquadFilterQ31;
class BiquadFilterF32;
class FirDecimateFilterQ31;

/*! \brief Complex Class
 * \details The Complex class is a template for holding
 * raw data types that comprise complex data.
 *
 */
template <typename T> class Complex : public api::DspInfoObject {
public:

	/*! \details Returns a reference to the real
	  * component (read/write).
	  */
	T & real(){ return m_value[0]; }

	/*! \details Returns a reference to the complex
	  * component (read/write).
	  *
	  */
	T & imaginary(){ return m_value[1]; }

	/*! \details Returns a reference to the real
	  * component (read-only).
	  */
	const T & real() const { return m_value[0]; }

	/*! \details Returns a reference to the imaginary
	  * component (read-only).
	  */
	const T & imaginary() const { return m_value[1]; }

	/*! \details Adds this to a and returns a new object. */
	T operator + (const T & a) const {
		T value;
		value.real() = real() + a.real();
		value.imaginary() = imaginary() + a.imaginary();
		return value;
	}

	/*! \details Adds \a a to this object. */
	T & operator += (const T & a){
		real() += a.real();
		imaginary() += a.imaginary();
		return *this;
	}

	/*! \details Subtracts this to a and returns a new object. */
	T operator - (const T & a) const {
		T value;
		value.real() = real() - a.real();
		value.imaginary() = imaginary() - a.imaginary();
		return value;
	}

	/*! \details Subtracts \a a from this object. */
	T & operator -= (const T & a){
		real() -= a.real();
		imaginary() -= a.imaginary();
		return *this;
	}

	bool operator == (const T & a) const {
		return (real() == a.real()) && (imaginary() == a.imaginary());
	}

	bool operator != (const T & a) const {
		return (real() != a.real()) && (imaginary() != a.imaginary());
	}

private:
	T m_value[2];
};

/*! \brief Complex Q15 Value
 * \details The ComplexQ15 represents a single
 * complex value in the q1.15 fixed point format.
 *
 */
class ComplexQ15 : public Complex<q15_t> {
public:

	/*! \details Constructs a new value.
	  *
	  * @param r real value
	  * @param imag imaginary value
	  */
	ComplexQ15(q15_t r = 0, q15_t imag = 0){
		real() = r; imaginary() = imag;
	}


};

/*! \brief Complex Q31 Value
 * \details The ComplexQ31 represents a single
 * complex value in the q1.31 fixed point format.
 *
 */
class ComplexQ31 : public Complex<q31_t> {
public:
	ComplexQ31(q31_t r = 0, q31_t imag = 0){
		real() = r; imaginary() = imag;
	}
};

/*! \brief Complex F32 Value
 * \details The ComplexF32 represents a single
 * complex value in the 32-bit floating point format.
 *
 */
class ComplexF32 : public Complex<float32_t> {
public:
	ComplexF32(float32_t r = 0, float32_t imag = 0){
		real() = r; imaginary() = imag;
	}
};

/*! \brief Signal Data Template
 * \details The SignalData class is a template
 * class for signal data vectors used in DSP
 * processing algorithms.
 *
 * All signals are dynamically allocated using the var::Vector
 * class.
 *
 */
template<class Derived, typename T, typename BigType> class SignalData : public var::Vector<T>, public api::DspWorkObject {
public:

	SignalData(){}
	SignalData(size_t count) : var::Vector<T>(count){
		if( is_api_available() == false ){
			//request_arm_dsp_api();
		}
	}

	virtual bool is_api_available() const {
		return false;
	}

	/*! \details Performs element-wise addition.
	  *
	  * Operators are not implemented on complex signals.
	  *
	  * \note This operator uses dynamic memory allocation
	  *
	  */
	Derived operator + (const Derived & a ) const { return add(a); }

	/*! \details Performs element-wise addition.
	  *
	  * Operators are not implemented on complex signals.
	  */
	Derived & operator += (const Derived & a ){ return add_assign(a); }

	/*! \details Adds a constant value to all elements.
	  *
	  * Operators are not implemented on complex signals.
	  *
	  * \note This operator uses dynamic memory allocation
	  */
	Derived operator + (const T & a ) const { return add(a); }

	/*! \details Adds a constant value to all elements in this signal. */
	Derived & operator += (const T & a ){ return add_assign(a); }

	/*! \details Performs element-wise subtraction.
	  *
	  * Operators are not implemented on complex signals.
	  *
	  * \note This operator uses dynamic memory allocation
	  */
	Derived operator - (const Derived & a) const { return subtract(a); }

	/*! \details Performs element-wise subtraction and saves the result in this signal.
	  *
	  *
	  * Operators are not implemented on complex signals.
	  *
	  *
	  */
	Derived & operator -= (const Derived & a){ return subtract_assign(a); }

	/*! \details Subtracts a scalar value from each element.
	  *
	  *
	  * Operators are not implemented on complex signals.
	  *
	  *
	  * \note This operator uses dynamic memory allocation
	  */
	Derived operator - (const T & a) const { return add(-a); }

	/*! \details Subtracts a scalar value from each element in this signal.
	  *
	  * Operators are not implemented on complex signals.
	  *
	  */
	Derived & operator -= (const T & a){ return add_assign(-a); }

	/*!
	  * \details Calculates element-by-element multiplication.
	  *
	  * \param a The second operand of the multiply operation
	  * \return A new signal containing the product of \a a and this signal.
	  *
	  * This operator requires the use of dynamic memory allocation. operator *=() does not.
	  *
	  */
	Derived operator * (const Derived & a ) const { return multiply(a); }

	/*! \details Multiples this with \a and stores the result in this signal.
	  *
	  * @param a The signal to multiply with.
	  *
	  * This is faster than operator *() because no dynamic memory allocation is used.
	  *
	  */
	Derived & operator *= (const Derived & a ){ return multiply_assign(a); }

	/*! \details Multiplies each element by a scaling value.
	  *
	  *
	  * Operators are not implemented on complex signals.
	  *
	  */
	Derived operator * (const T & value) const { return multiply(value); }

	/*! \details Multiplies each element of this signal by a scalar value.
	  *
	  *
	  * Operators are not implemented on complex signals.
	  *
	  */
	Derived & operator *= (const T & value){ return multiply_assign(value); }

	/*! \details Shifts this signal.
	  *
	  * @param value Number of bits to left shift
	  * @return A new signal with each element equal to this << value
	  *
	  * \note This operator uses dynamic memory allocation.
	  *
	  * Operators are not implemented on complex signals. Shift is not available for any floating-point types.
	  *
	  */
	Derived operator << (s8 value) const { return shift(value); }

	/*! \details Shifts this signal.
	  *
	  * @param value Number of bits to left shift
	  * @return A new signal with each element equal to this >> value
	  *
	  * Operators are not implemented on complex signals. Shift is not available for any floating-point types.
	  *
	  * \note This operator uses dynamic memory allocation.
	  *
	  *
	  */
	Derived operator >> (s8 value) const { return shift(-1*value); }

	/*! \details Shifts this signal \a value bits to the left.
	  *
	  * Operators are not implemented on complex signals. Shift is not available for any floating-point types.
	  *
	  */
	Derived & operator <<= (s8 value){ return shift_assign(value); }

	/*! \details Shifts this signal \a value bits to the right.
	  *
	  * Operators are not implemented on complex signals. Shift is not available for any floating-point types.
	  *
	  */
	Derived & operator >>= (s8 value){ return shift_assign(-1*value); }

	/*! \details Returns true if this signal is equivalent as \a a. */
	bool operator == (const Derived & a) const {
		return this->compare(*this, a);
	}

	/*! \details Returns true if this signal is not equivalent as \a a. */
	bool operator != (const Derived & a) const {
		return !this->compare(*this, a);
	}


#if defined __DOXYGEN__
	/*! \details Returns the mean value of the signal.
	  *
	  * \code
	  * SignalQ31 data(16);
	  *
	  * data.fill(10);
	  * printf("Mean is %ld\n", data.mean());
	  * \endcode
	  *
	  */
	virtual T mean() const;

	virtual BigType power() const;

	/*! \details Calculates and returns the variance value of the signal.
	  *
	  * This method utilizes the arm_var_q15() function.
	  *
	  *
	  */
	virtual T variance() const;

	/*! \details Calculates the RMS value of the signal. */
	virtual T rms() const;

	/*! \details Calculates and returns the standard deviation value of the signal.
	  *
	  * This method utilizes the arm_std_q15() function.
	  */
	virtual T std() const;

	/*! \details Calculates and returns the minimum value of the signal.
	  *
	  * This method utilizes the arm_min_q15() function.
	  */
	virtual T min() const;

	/*! \details Calculates and returns the minimum value of the signal.
	  *
	  * @param idx A reference to the index value of the minimum. This parameter
	  * will be written with the location of the minimum value.
	  *
	  * This method utilizes the arm_min_q15() function.
	  */
	virtual T min(u32 & idx) const;

	/*! \details Calculates and returns the maximum value of the signal.
	  *
	  * This method utilizes the arm_max_q15() function.
	  */
	virtual T max() const;

	/*! \details Calculates and returns the maximum value of the signal.
	  *
	  * @param idx A reference to the index value of the maximum. This parameter
	  * will be written with the location of the maximum value.
	  *
	  * This method utilizes the arm_max_q15() function.
	  */
	virtual T max(u32 & idx) const;

	/*!
	  * \details Calculates the absolute value of each value in the signal.
	  * \return A new signal containing the absolute value of this signal.
	  *
	  * This method will allocate a new signal and return it.
	  *
	  */
	virtual SignalData<T, BigType> abs() const;

	/*!
	  * \details Calculates the absolute value of each value in the signal.
	  * \param output A reference to the destination signal
	  * \return A new signal containing the absolute value of this signal.
	  *
	  */
	virtual void abs(SignalData & output) const;

	/*!
	  * \brief Calculates and returns the dot product of this signal and the the parameter passed.
	  *
	  * \param a Used as the second signal used to calculate the dot product.
	  * \return Returns the dot product of \a a and this signal.
	  */
	virtual BigType dot_product(const SignalData & a) const;

	/*! \details Adds an offset to each element in the signal. */
	virtual SignalData offset(T offset_value) const;

	/*! \details Returns a new signal that a negated copy of this signal.
	  *
	  * This method uses dynamic memory allocation.
	  *
	  */
	virtual SignalData negate() const;

	/*! \details Assigns the negated value of this signal to \a output. */
	virtual void negate(SignalData & output) const;

	virtual SignalData convolve(const SignalData & a) const;

	virtual void convolve(SignalData & output, const SignalData & a) const;

	virtual SignalData scale(T scale_fraction, s8 shift = 0);
	virtual void scale(SignalData & output, T scale_fraction, s8 shift = 0);


	/*! \details Filters the signal.
	  *
	  * @param filter Biquad Filter to use
	  *
	  * \note This method uses dynamic memory allocation.
	  *
	  */
	virtual SignalData filter(const BiquadFilterType & filter) const;

	/*! \details Filters the signal.
	  *
	  * @param output A reference to the output signal
	  * @param filter Biquad Filter to use
	  *
	  *
	  */
	virtual void filter(SignalData & output, const BiquadFilterType & filter) const;

	/*! \details Filters the signal.
	  *
	  * @param filter An FIR filter to apply to the signal
	  *
	  * \note This method uses dynamic memory allocation.
	  *
	  */
	virtual SignalData filter(const FirFilterType & filter) const;

	/*! \details Filters the signal.
	  *
	  * @param output A reference to the output signal
	  * @param filter An FIR filter to apply to the signal
	  *
	  *
	  */
	void filter(SignalData & output, const FirFilterType & filter) const;
#endif

	//these are used by operators and are not documented
	virtual Derived shift(s8 value) const { return Derived(); }
	virtual Derived & shift_assign(s8 value){ return (Derived&)*this; }
	virtual Derived add(const Derived & a) const {
		return Derived();
	}
	virtual Derived & add_assign(const Derived & a){ return (Derived&)*this; }
	virtual Derived add(const T & a) const { return Derived(); }
	virtual Derived & add_assign(const T & a){ return (Derived&)*this; }
	virtual Derived subtract(const Derived & a) const { return Derived(); }
	virtual Derived & subtract_assign(const Derived & a){ return (Derived&)*this; }
	virtual Derived multiply(const Derived & a) const { return Derived(); }
	virtual Derived & multiply_assign(const Derived & a){ return (Derived&)*this; }
	virtual Derived multiply(const T & scale_fraction) const { return Derived(); }
	virtual Derived & multiply_assign(const T & scale_fraction){ return (Derived&)*this; }

protected:

private:

};

#if 0

typedef SignalData<q7_t, q63_t> SignalDataQ7;

class SignalQ7 : public SignalDataQ7 {
public:
   SignalQ7(size_t count) : SignalData<q7_t>(count){}

};
#endif


/*! \brief Signal q1.15
 * \details This class holds signal data in the q1.15
 * fixed-point format.
 *
 */
class SignalQ15 : public SignalData<SignalQ15, q15_t, q63_t> {
public:
	SignalQ15(size_t count) : SignalData(count){}
	SignalQ15(){}

	bool is_api_available() const {
		return api_q15().is_valid();
	}

	q15_t mean() const;
	q63_t power() const;
	q15_t variance() const;
	q15_t rms() const;
	q15_t std() const;
	q15_t min() const;
	q15_t min(u32 & idx) const;
	q15_t max() const;
	q15_t max(u32 & idx) const;
	SignalQ15 abs() const;
	void abs(SignalQ15 & output) const;
	q63_t dot_product(const SignalQ15 & a) const;
	SignalQ15 negate() const;
	void negate(SignalQ15 & output) const;
	SignalQ15 convolve(const SignalQ15 & a) const ;
	void convolve(SignalQ15 & output, const SignalQ15 & a) const ;
	void shift(SignalQ15 & output, s8 value) const;
	SignalQ15 scale(q15_t scale_fraction, s8 shift = 0) const;
	void scale(SignalQ15 & output, q15_t scale_fraction, s8 shift = 0) const;


	SignalQ15 add(q15_t offset_value) const;
	SignalQ15 & add_assign(q15_t offset_value);
	SignalQ15 add(const SignalQ15 & a) const;
	SignalQ15 & add_assign(const SignalQ15 & a);
	SignalQ15 shift(s8 value) const;
	SignalQ15 & shift_assign(s8 value);
	SignalQ15 multiply(q15_t value) const;
	SignalQ15 & multiply_assign(q15_t value);
	SignalQ15 multiply(const SignalQ15 & a) const;
	SignalQ15 & multiply_assign(const SignalQ15 & a);
	SignalQ15 subtract(const SignalQ15 & a) const;
	SignalQ15 & subtract_assign(const SignalQ15 & a);


	//Filters
	SignalQ15 filter(const FirFilterQ15 & filter) const;
	void filter(SignalQ15 & output, const FirFilterQ15 & filter) const;

	SignalQ15 filter(const BiquadFilterQ15 & filter) const;
	void filter(SignalQ15 & output, const BiquadFilterQ15 & filter) const;

	//SignalQ15 filter(const FirDecimateFilterQ15 & filter);
	//void filter(SignalQ15 & output, const FirDecimateFilterQ15 & filter);

	static SignalQ15 create_sin_wave(u32 wave_freauency, u32 sampling_frequency, u32 nsamples, q15_t phase = 0);

private:


};

/*!
 * \brief Signal with Complex q1.15 data
 * \details This class is used for storing complex signal data in q1.15
 * format.
 *
 * Complex data is required when performing transform operations (FFT and DCT).
 *
 *
 */
class SignalComplexQ15 : public SignalData<SignalComplexQ15, ComplexQ15, q63_t> {
public:

	SignalComplexQ15(){}
	SignalComplexQ15(size_t count) : SignalData(count){}

	bool is_api_available() const {
		return api_q15().is_valid();
	}

	SignalComplexQ15 transform(FftRealQ15 & fft, bool is_inverse = false);
	void transform(SignalComplexQ15 & output, FftRealQ15 & fft, bool is_inverse = false);
	void transform(FftComplexQ15 & fft, bool is_inverse = false, bool is_bit_reversal = false);


protected:

private:

};


/*! \brief Fixed point q1.31 data signal
 * \details The SignalQ31 class stores
 * a real q1.31 data signal.
 *
 *
 */
class SignalQ31 : public SignalData<SignalQ31, q31_t, q63_t> {
public:

	/*! \details Constructs a signal with uninitialized
	  * values.
	  *
	  * @param count The number of data points
	  *
	  */
	SignalQ31(size_t count) : SignalData(count){}

	bool is_api_available() const {
		return api_q31().is_valid();
	}

	/*! \details Contructs an empty signal. */
	SignalQ31(){}


	q31_t mean() const;
	q63_t power() const;
	q31_t variance() const;
	q31_t rms() const;
	q31_t std() const;
	q31_t min() const;
	q31_t min(u32 & idx) const;
	q31_t max() const;
	q31_t max(u32 & idx) const;
	SignalQ31 abs() const;
	void abs(SignalQ31 & output) const;
	q63_t dot_product(const SignalQ31 & a) const;
	SignalQ31 negate() const ;
	void negate(SignalQ31 & output) const ;
	SignalQ31 convolve(const SignalQ31 & a) const ;
	void convolve(SignalQ31 & output, const SignalQ31 & a) const ;
	void shift(SignalQ31 & output, s8 value) const;
	SignalQ31 scale(q31_t scale_fraction, s8 shift = 0) const;
	void scale(SignalQ31 & output, q31_t scale_fraction, s8 shift = 0) const;

	SignalQ31 add(q31_t offset_value) const;
	SignalQ31 & add_assign(q31_t offset_value);
	SignalQ31 add(const SignalQ31 & a) const;
	SignalQ31 & add_assign(const SignalQ31 & a);
	SignalQ31 shift(s8 value) const ;
	SignalQ31 & shift_assign(s8 value);
	SignalQ31 multiply(q31_t value) const;
	SignalQ31 & multiply_assign(q31_t value);
	SignalQ31 multiply(const SignalQ31 & a) const;
	SignalQ31 & multiply_assign(const SignalQ31 & a);
	SignalQ31 subtract(const SignalQ31 & a) const;
	SignalQ31 & subtract_assign(const SignalQ31 & a);

	SignalQ31 filter(const FirFilterQ31 & filter) const;
	void filter(SignalQ31 & output, const FirFilterQ31 & filter) const;
	SignalQ31 filter(const BiquadFilterQ31 & filter) const;
	void filter(SignalQ31 & output, const BiquadFilterQ31 & filter) const;

	SignalQ31 filter(const FirDecimateFilterQ31 & filter);
	void filter(SignalQ31 & output, const FirDecimateFilterQ31 & filter);
	//void filter(const FirInterpolateFilterQ31 & filter);
	//void filter(const FirLatticeFilterQ31 & filter);
	//void filter(const IirLatticeFilterQ31 & filter);
	//void filter(const BiquadCascadeFilterQ31 & filter);
	//void filter(const LmsFilterQ31 & filter);
	//void filter(const LmsNormalFilterQ31 & filter);

	static SignalQ31 create_sin_wave(u32 wave_freauency, u32 sampling_frequency, u32 nsamples, q31_t phase = 0);

private:

};

/*!
 * \brief Signal with Complex q1.31 Data
 * \details This class is used for storing complex signal data in q1.31
 * format.
 *
 * Complex data is required when performing transform operations (FFT and DCT).
 *
 *
 */
class SignalComplexQ31 : public SignalData<SignalComplexQ31, ComplexQ31, q63_t> {
public:

	SignalComplexQ31(){}
	SignalComplexQ31(size_t count) : SignalData(count){}

	bool is_api_available() const {
		return api_q31().is_valid();
	}

	/*! \details Transforms this object and returns a new signal with the transformed data.
	  */
	SignalComplexQ31 transform(FftRealQ31 & fft, bool is_inverse = false);
	void transform(SignalComplexQ31 & output, FftRealQ31 & fft, bool is_inverse = false);

	void transform(FftComplexQ31 & fft, bool is_inverse = false, bool is_bit_reversal = false);


protected:
	//const q31_t * vector_data_const() const { return (const q31_t*)SignalData<ComplexQ31>::vector_data_const(); }
	//q31_t * vector_data(){ return (q31_t*)SignalData<ComplexQ31>::vector_data(); }


private:
	friend class FftRealQ31;
	friend class FftComplexQ31;
};


/*! \brief Signal with 32-bit Floating Point values
 * \details This class holds a 32-bit floating point
 * signal with real values.
 *
 * \code
 * #include <sapi/dsp.hpp>
 *
 * SignalF32 a(32);
 * SignalF32 b(32);
 * a.fill(0);
 * b.fill(5);
 * a += b; //add elements from b to a
 * a += 10.0f; //offset a by 10.0f
 * printf("A mean is %0.2f\n", a.mean());
 * \endcode
 *
 *
 */
class SignalF32 : public SignalData<SignalF32, float32_t, float32_t> {
public:
	SignalF32(size_t count) : SignalData(count){}
	SignalF32(){}

	bool is_api_available() const {
		return api_f32().is_valid();
	}

	float32_t mean() const;
	float32_t power() const;
	float32_t variance() const;
	float32_t rms() const;
	float32_t std() const;
	float32_t min() const;
	float32_t min(u32 & idx) const;
	float32_t max() const;
	float32_t max(u32 & idx) const;
	SignalF32 abs() const;
	void abs(SignalF32 & output) const;
	float32_t dot_product(const SignalF32 & a) const;
	SignalF32 negate() const ;
	void negate(SignalF32 & output) const ;
	SignalF32 convolve(const SignalF32 & a) const ;
	void convolve(SignalF32 & output, const SignalF32 & a) const ;
	void shift(SignalF32 & output, s8 value) const;
	SignalF32 scale(float32_t scale_fraction, s8 shift = 0) const;
	void scale(SignalF32 & output, float32_t scale_fraction, s8 shift = 0) const;

	SignalF32 add(float32_t offset_value) const;
	SignalF32 & add_assign(float32_t offset_value);
	SignalF32 add(const SignalF32 & a) const;
	SignalF32 & add_assign(const SignalF32 & a);
	//SignalDataF32 shift(s8 value) const;
	//SignalDataF32 & shift_assign(s8 value);
	SignalF32 multiply(float32_t value) const;
	SignalF32 & multiply_assign(float32_t value);
	SignalF32 multiply(const SignalF32 & a) const;
	SignalF32 & multiply_assign(const SignalF32 & a);
	SignalF32 subtract(const SignalF32 & a) const;
	SignalF32 & subtract_assign(const SignalF32 & a);

	//Filters
	SignalF32 filter(const FirFilterF32 & filter) const;
	void filter(SignalF32 & output, const FirFilterF32 & filter) const;

	SignalF32 filter(const BiquadFilterF32 & filter) const;
	void filter(SignalF32 & output, const BiquadFilterF32 & filter) const;

	static SignalF32 create_sin_wave(float32_t wave_freauency, float32_t sampling_frequency, u32 nsamples, float32_t phase = 0);

private:


};

typedef SignalF32 SignalFloat32;

class SignalComplexF32 : public SignalData<SignalComplexF32, ComplexF32, float32_t> {
public:

	SignalComplexF32(){}
	SignalComplexF32(size_t count) : SignalData(count){}

	bool is_api_available() const {
		return api_f32().is_valid();
	}

	SignalComplexF32 transform(FftRealF32 & fft, bool is_inverse = false);
	void transform(SignalComplexF32 & output, FftRealF32 & fft, bool is_inverse = false);

	void transform(FftComplexF32 & fft, bool is_inverse = false, bool is_bit_reversal = false);


protected:
private:

};

}

#endif // SAPI_DSP_SIGNAL_DATA_HPP_
