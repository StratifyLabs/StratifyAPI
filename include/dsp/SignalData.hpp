#ifndef DSP_SIGNAL_DATA_HPP
#define DSP_SIGNAL_DATA_HPP

#include "../api/DspObject.hpp"
#include "../var/Vector.hpp"

namespace dsp {

class FftComplexQ15;
class FftComplexQ31;
class FftComplexF32;
class FftRealQ15;
class FftRealQ31;
class FftRealF32;
class FirFilterQ31;
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
template<typename T, typename BigType> class SignalData : public var::Vector<T>, public api::DspWorkObject {
public:
    SignalData(){}
    SignalData(int count) : var::Vector<T>(count){}

    /*! \details Performs element-wise addition.
     *
     * \note This operator uses dynamic memory allocation
     *
     */
    SignalData operator + (const SignalData & a ) const { return add(a); }

    /*! \details Performs element-wise addition.
     *
     */
    SignalData & operator += (const SignalData & a ){ return add_assign(a); }

    /*! \details Adds a constant value to all elements.
     * \note This operator uses dynamic memory allocation
     */
    SignalData operator + (const T & a ) const { return add(a); }
    /*! \details Adds a constant value to all elements in this signal. */
    SignalData & operator += (const T & a ){ return add_assign(a); }
    SignalData operator - (const SignalData & a ) const { return subtract(a); }
    SignalData & operator -= (const SignalData & a ){ return subtract_assign(a); }

    SignalData operator - (const T & a ) const { return add(-a); }
    SignalData & operator -= (const T & a ){ return add_assign(-a); }

    /*!
     * \details Calculates element-by-element multiplication.
     *
     * \param a The second operand of the multiply operation
     * \return A new signal containing the product of \a a and this signal.
     *
     * This operator requires the use of dynamic memory allocation. operator *=() does not.
     *
     */
    SignalData operator * (const SignalData & a ) const { return multiply(a); }

    /*! \details Multiples this with \a and stores the result in this signal.
     *
     * @param a The signal to multiply with.
     *
     * This is faster than operator *() because no dynamic memory allocation is used.
     *
     */
    SignalData & operator *= (const SignalData & a ){ return multiply_assign(a); }

    /*! \details Multiplies each element by a scaling value. */
    SignalData operator * (const T & value) const { return multiply(value); }

    /*! \details Multiplies each element of this signal by a scalar value. */
    SignalData & operator *= (const T & value){ return multiply_assign(value); }

    /*! \details Shifts this signal.
     *
     * @param value Number of bits to left shift
     * @return A new signal with each element equal to this << value
     *
     * \note This operator uses dynamic memory allocation.
     *
     */
    SignalData operator << (s8 value) const { return shift(value); }

    /*! \details Shifts this signal.
     *
     * @param value Number of bits to left shift
     * @return A new signal with each element equal to this >> value
     *
     * \note This operator uses dynamic memory allocation.
     *
     */
    SignalData operator >> (s8 value) const { return shift(-1*value); }

    /*! \details Shifts this signal \a value bits to the left. */
    SignalData & operator <<= (s8 value){ return shift_assign(value); }
    /*! \details Shifts this signal \a value bits to the right. */
    SignalData & operator >>= (s8 value){ return shift_assign(-1*value); }

    /*! \details Returns true if this signal is equivalent as \a a. */
    bool operator == (const SignalData & a) const {
        return this->compare(*this, a);
    }

    /*! \details Returns true if this signal is not equivalent as \a a. */
    bool operator != (const SignalData & a) const {
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
     * \note This method using dynamic memory allocation.
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
#endif

    //these are used by operators and are not documented
    virtual SignalData shift(s8 value) const { return SignalData(); }
    virtual SignalData & shift_assign(s8 value){ return *this; }
    virtual SignalData add(const SignalData & a) const { return SignalData(); }
    virtual SignalData & add_assign(const SignalData & a){ return *this; }
    virtual SignalData add(const T & a) const { return SignalData(); }
    virtual SignalData & add_assign(const T & a){ return *this; }
    virtual SignalData subtract(const SignalData & a) const { return SignalData(); }
    virtual SignalData & subtract_assign(const SignalData & a){ return *this; }
    virtual SignalData multiply(const SignalData & a) const { return SignalData(); }
    virtual SignalData & multiply_assign(const SignalData & a){ return *this; }
    virtual SignalData multiply(const T & scale_fraction) const { return SignalData(); }
    virtual SignalData & multiply_assign(const T & scale_fraction){ return *this; }

protected:

private:

};

#if 0

typedef SignalData<q7_t, q63_t> SignalDataQ7;

class SignalQ7 : public SignalDataQ7 {
public:
    SignalQ7(int count) : SignalData<q7_t>(count){}

};
#endif

typedef SignalData<q15_t, q63_t> SignalDataQ15;

/*! \brief Signal q1.15
 * \details This class holds signal data in the q1.15
 * fixed-point format.
 *
 */
class SignalQ15 : public SignalDataQ15 {
public:
    SignalQ15(int count) : SignalData(count){}
    SignalQ15(){}

    SignalQ15(const SignalData & a){
        (SignalData &)*this = a;
    }

    SignalQ15 & operator = (const SignalData & a){
        (SignalData &)*this = a;
        return *this;
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


    SignalDataQ15 add(q15_t offset_value) const;
    SignalDataQ15 & add_assign(q15_t offset_value);
    SignalDataQ15 add(const SignalQ15 & a) const;
    SignalDataQ15 & add_assign(const SignalQ15 & a);
    SignalDataQ15 shift(s8 value) const;
    SignalDataQ15 & shift_assign(s8 value);
    SignalDataQ15 multiply(q15_t value) const;
    SignalDataQ15 & multiply_assign(q15_t value);
    SignalDataQ15 multiply(const SignalQ15 & a) const;
    SignalDataQ15 & multiply_assign(const SignalQ15 & a);
    SignalDataQ15 subtract(const SignalQ15 & a) const;
    SignalDataQ15 & subtract_assign(const SignalQ15 & a);


    //Filters
    //SignalQ15 filter(const FirFilterQ15 & filter) const;

    //void filter(SignalQ15 & output, const FirFilterQ15 & filter) const;

    SignalQ15 filter(const BiquadFilterQ15 & filter) const;
    void filter(SignalQ15 & output, const BiquadFilterQ15 & filter) const;

    //SignalQ15 filter(const FirDecimateFilterQ15 & filter);
    //void filter(SignalQ15 & output, const FirDecimateFilterQ15 & filter);

private:


};

typedef SignalData<ComplexQ15, q63_t> SignalDataComplexQ15;

/*!
 *
 * \details This class is used for storing complex signal data in q1.15
 * format.
 *
 * Complex data is required when performing transform operations (FFT and DCT).
 *
 *
 */
class SignalComplexQ15 : public SignalDataComplexQ15 {
public:

    SignalComplexQ15(){}
    SignalComplexQ15(u32 count) : SignalData(count){}

    SignalComplexQ15(const SignalData & a){
        (SignalData&)*this = a;
    }

    SignalComplexQ15 & operator = (const SignalData & a){
        (SignalData&)*this = a;
        return *this;
    }

    SignalComplexQ15 transform(FftRealQ15 & fft, bool is_inverse = false);
    void transform(SignalComplexQ15 & output, FftRealQ15 & fft, bool is_inverse = false);
    void transform(FftComplexQ15 & fft, bool is_inverse = false, bool is_bit_reversal = false);


protected:

private:

};


typedef SignalData<q31_t, q63_t> SignalDataQ31;

/*! \brief Fixed point q1.31 data signal
 * \details The SignalQ31 class stores
 * a real q1.31 data signal.
 *
 *
 */
class SignalQ31 : public SignalDataQ31 {
public:

    /*! \details Constructs a signal with uninitialized
     * values.
     *
     * @param count The number of data points
     *
     */
    SignalQ31(int count) : SignalData(count){}

    /*! \details Contructs an empty signal. */
    SignalQ31(){}

    SignalQ31(const SignalData & a){
        (SignalData&)*this = a;
    }

    SignalQ31 & operator = (const SignalData & a){
        (SignalData&)*this = a;
        return *this;
    }

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

    SignalDataQ31 add(q31_t offset_value) const;
    SignalDataQ31 & add_assign(q31_t offset_value);
    SignalDataQ31 add(const SignalQ31 & a) const;
    SignalDataQ31 & add_assign(const SignalQ31 & a);
    SignalDataQ31 shift(s8 value) const ;
    SignalDataQ31 & shift_assign(s8 value);
    SignalDataQ31 multiply(q31_t value) const;
    SignalDataQ31 & multiply_assign(q31_t value);
    SignalDataQ31 multiply(const SignalQ31 & a) const;
    SignalDataQ31 & multiply_assign(const SignalQ31 & a);
    SignalDataQ31 subtract(const SignalQ31 & a) const;
    SignalDataQ31 & subtract_assign(const SignalQ31 & a);

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

    static SignalQ31 create_sin_wave(u32 wave_freauency, u32 sampling_frequency, u32 samples, q31_t phase = 0);

private:

};

typedef SignalData<ComplexQ31, q63_t> SignalDataComplexQ31;

/*!
 *
 * \details This class is used for storing complex signal data in q1.31
 * format.
 *
 * Complex data is required when performing transform operations (FFT and DCT).
 *
 *
 */
class SignalComplexQ31 : public SignalDataComplexQ31 {
public:

    SignalComplexQ31(){}
    SignalComplexQ31(u32 count) : SignalData(count){}

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

typedef SignalData<float32_t, float32_t> SignalDataF32;

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
class SignalF32 : public SignalDataF32 {
public:
    SignalF32(int count) : SignalData(count){}
    SignalF32(){}

    SignalF32(const SignalData & a){
        (SignalData&)*this = a;
    }

    SignalF32 & operator = (const SignalData & a){
        (SignalData&)*this = a;
        return *this;
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

    SignalDataF32 add(float32_t offset_value) const;
    SignalDataF32 & add_assign(float32_t offset_value);
    SignalDataF32 add(const SignalF32 & a) const;
    SignalDataF32 & add_assign(const SignalF32 & a);
    //SignalDataF32 shift(s8 value) const;
    //SignalDataF32 & shift_assign(s8 value);
    SignalDataF32 multiply(float32_t value) const;
    SignalDataF32 & multiply_assign(float32_t value);
    SignalDataF32 multiply(const SignalF32 & a) const;
    SignalDataF32 & multiply_assign(const SignalF32 & a);
    SignalDataF32 subtract(const SignalF32 & a) const;
    SignalDataF32 & subtract_assign(const SignalF32 & a);

    //Filters
    //SignalF32 filter(const FirFilterF32 & filter) const;
    //void filter(SignalF32 & output, const FirFilterF32 & filter) const;

    SignalF32 filter(const BiquadFilterF32 & filter) const;
    void filter(SignalF32 & output, const BiquadFilterF32 & filter) const;

private:


};

typedef SignalF32 SignalFloat32;

class SignalComplexF32 : public SignalData<ComplexF32, float32_t> {
public:

    SignalComplexF32(){}
    SignalComplexF32(u32 count) : SignalData(count){}

    SignalComplexF32 transform(FftRealF32 & fft, bool is_inverse = false);
    void transform(SignalComplexF32 & output, FftRealF32 & fft, bool is_inverse = false);

    void transform(FftComplexF32 & fft, bool is_inverse = false, bool is_bit_reversal = false);


protected:
private:

};

}

#endif // DSP_SIGNAL_DATA_HPP
