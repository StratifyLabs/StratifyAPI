#ifndef DSP_SIGNAL_DATA_HPP
#define DSP_SIGNAL_DATA_HPP

#include "../api/DspObject.hpp"
#include "../var/Vector.hpp"

namespace dsp {

class ComplexFftQ15;
class RealFftQ15;
class ComplexFftQ31;
class RealFftQ31;
class ComplexFftF32;
class RealFftF32;
class FirFilterQ31;
class BiquadDirectFormOneFilterQ15;
class BiquadDirectFormOneFilterQ31;
class BiquadDirectFormOneFilterF32;
class FirDecimateFilterQ31;

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

private:
    T m_value[2];
};

/*! \brief Complex Q31 Value
 * \details The ComplexQ31 represents a single
 * complex value in the 1.31 fixed point format.
 *
 */
class ComplexQ31 : public Complex<q31_t> {
public:

    ComplexQ31(q31_t r = 0, q31_t imag = 0){
        real() = r; imaginary() = imag;
    }

    ComplexQ31 operator + (const ComplexQ31 & a) const {
        ComplexQ31 value;
        value.real() = real() + a.real();
        value.imaginary() = imaginary() + a.imaginary();
        return value;
    }

    ComplexQ31 & operator += (const ComplexQ31 & a){
        real() += a.real();
        imaginary() += a.imaginary();
        return *this;
    }

    ComplexQ31 operator - (const ComplexQ31 & a) const {
        ComplexQ31 value;
        value.real() = real() - a.real();
        value.imaginary() = imaginary() - a.imaginary();
        return value;
    }

    ComplexQ31 & operator -= (const ComplexQ31 & a){
        real() -= a.real();
        imaginary() -= a.imaginary();
        return *this;
    }

    bool operator == (const ComplexQ31 & a) const {
        return (real() == a.real()) && (imaginary() == a.imaginary());
    }

    bool operator != (const ComplexQ31 & a) const {
        return (real() != a.real()) && (imaginary() != a.imaginary());
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
template<typename T> class SignalData : public var::Vector<T>, public api::DspWorkObject {
public:
    SignalData(int count){
        var::Vector<T>::resize(count);
    }

    SignalData(){}

private:

};

class SignalQ7 : public SignalData<q7_t> {
public:
    SignalQ7(int count) : SignalData<q7_t>(count){}

};

class SignalQ15 : public SignalData<q15_t> {
public:
    SignalQ15(int count) : SignalData<q15_t>(count){}
    SignalQ15(){}

    /*! \details Calculates and returns the mean value of the signal.
     *
     * This method utilizes the arm_mean_q15() function.
     *
     * \code
     * SignalQ15 data(16);
     * data.fill(100);
     * printf("Mean is %ld\n", data.mean());
     * \endcode
     */
    q15_t mean() const {
        q15_t result;
        arm_dsp_api_q15()->mean((q15_t*)data_const(), count(), &result);
        return result;
    }

    q63_t power() const {
        q63_t result;
        arm_dsp_api_q15()->power((q15_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the variance value of the signal.
     *
     * This method utilizes the arm_var_q15() function.
     *
     *
     */
    q15_t variance() const {
        q15_t result;
        arm_dsp_api_q15()->var((q15_t*)data_const(), count(), &result);
        return result;
    }

    q15_t rms() const {
        q15_t result;
        arm_dsp_api_q15()->rms((q15_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the standard deviation value of the signal.
     *
     * This method utilizes the arm_std_q15() function.
     */
    q15_t std() const {
        q15_t result;
        arm_dsp_api_q15()->std((q15_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the minimum value of the signal.
     *
     * This method utilizes the arm_min_q15() function.
     */
    q15_t min() const {
        q15_t result;
        u32 idx;
        arm_dsp_api_q15()->min((q15_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the minimum value of the signal.
     *
     * @param idx A reference to the index value of the minimum. This parameter
     * will be written with the location of the minimum value.
     *
     * This method utilizes the arm_min_q15() function.
     */
    q15_t min(u32 & idx) const {
        q15_t result;
        arm_dsp_api_q15()->min((q15_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the maximum value of the signal.
     *
     * This method utilizes the arm_max_q15() function.
     */
    q15_t max() const {
        q15_t result;
        u32 idx;
        arm_dsp_api_q15()->max((q15_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the maximum value of the signal.
     *
     * @param idx A reference to the index value of the maximum. This parameter
     * will be written with the location of the maximum value.
     *
     * This method utilizes the arm_max_q15() function.
     */
    q15_t max(u32 & idx) const {
        q15_t result;
        arm_dsp_api_q15()->max((q15_t*)data_const(), count(), &result, &idx);
        return result;
    }


    //abs
    /*!
     * \details Calculates the absolute value of each value in the signal.
     * \return A new signal containing the absolute value of this signal.
     *
     * This method will allocate a new signal and return it.
     *
     */
    SignalQ15 abs() const {
        SignalQ15 ret(count());
        arm_dsp_api_q15()->abs(
                    (q15_t *)data_const(),
                    ret.vector_data(),
                    count());
        ret.set_transfer_ownership();
        return ret;
    }
    /*!
     * \details Calculates the absolute value of each value in the signal.
     * \param output A reference to the destination signal
     * \return A new signal containing the absolute value of this signal.
     *
     */
    void abs(SignalQ15 & output) const {
        arm_dsp_api_q15()->abs(
                    (q15_t *)data_const(),
                    output.vector_data(),
                    count());
    }

    //add
    SignalQ15 operator + (const SignalQ15 & a) const{
        SignalQ15 ret(count());
        arm_dsp_api_q15()->add((q15_t*)vector_data_const(), (q15_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalQ15 & operator += (const SignalQ15 & a){
        arm_dsp_api_q15()->add((q15_t*)vector_data_const(), (q15_t*)a.vector_data_const(), vector_data(), count());
        return *this;
    }

    //dot prod
    /*!
     * \brief Calculates and returns the dot product of this signal and the the parameter passed.
     *
     * \param a Used as the second signal used to calculate the dot product.
     * \return Returns the dot product of \a a and this signal.
     */
    q63_t dot_product(const SignalQ15 & a) const {
        q63_t result;
        arm_dsp_api_q15()->dot_prod((q15_t*)vector_data_const(), (q15_t*)a.vector_data_const(), count(), &result);
        return result;
    }

    //mult
    /*!
     * \details Calculates element-by-element multiplication.
     *
     * \param a The second operand of the multiply operation
     * \return A new signal containing the product of \a a and this signal.
     *
     */
    SignalQ15 operator * (const SignalQ15 & a) const {
        SignalQ15 ret(count());
        arm_dsp_api_q15()->mult((q15_t*)vector_data_const(), (q15_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }


    /*! \details Adds an offset to each element in the signal. */
    SignalQ15 offset(q15_t offset_value) const {
        SignalQ15 ret(count());
        arm_dsp_api_q15()->offset((q15_t*)vector_data_const(), offset_value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    /*! \details Returns a new signal that a negated copy of this signal.
     *
     * This method uses dynamic memory allocation.
     *
     */
    SignalQ15 negate() const {
        SignalQ15 ret(count());
        arm_dsp_api_q15()->negate((q15_t*)vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    /*! \details Assigns the negated value of this signal to \a output. */
    void negate(SignalQ15 & output) const {
        arm_dsp_api_q15()->negate((q15_t*)vector_data_const(), output.vector_data(), count());
    }


    SignalQ15 convolve(const SignalQ15 & a) const {
        SignalQ15 ret(count() + a.count() - 1);
        arm_dsp_api_q15()->conv_fast((q15_t*)vector_data_const(), count(), (q15_t*)a.vector_data_const(), a.count(), ret.vector_data());
        ret.set_transfer_ownership();
        return ret;
    }

    void convolve(SignalQ15 & output, const SignalQ15 & a) const {
        //check output length?
        arm_dsp_api_q15()->conv_fast((q15_t*)vector_data_const(), count(), (q15_t*)a.vector_data_const(), a.count(), output.vector_data());
    }


    SignalQ15 scale(q15_t scale_fraction, int8_t shift = 0){
        SignalQ15 ret(count());
        arm_dsp_api_q15()->scale((q15_t*)vector_data_const(), scale_fraction, shift, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    void scale(SignalQ15 & output, q15_t scale_fraction, int8_t shift = 0){
        arm_dsp_api_q15()->scale((q15_t*)vector_data_const(), scale_fraction, shift, output.vector_data(), count());
    }

    //shift
    SignalQ15 shift(int8_t value){
        SignalQ15 ret(count());
        arm_dsp_api_q15()->shift((q15_t*)vector_data_const(), value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    void shift(SignalQ15 & output, int8_t value){
        arm_dsp_api_q15()->shift((q15_t*)vector_data_const(), value, output.vector_data(), count());
    }

    SignalQ15 operator * (q15_t value){
        return scale(value);
    }

    //subtract
    SignalQ15 operator - (const SignalQ15 & a) const{
        SignalQ15 ret(count());
        arm_dsp_api_q15()->sub((q15_t*)vector_data_const(), (q15_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalQ15 & operator -= (const SignalQ15 & a){
        arm_dsp_api_q15()->sub((q15_t*)vector_data_const(), (q15_t*)a.vector_data_const(), vector_data(), count());
        return *this;
    }

    bool operator == (const SignalQ15 & a) const {
        return compare(*this, a);
    }

    bool operator != (const SignalQ15 & a) const {
        return !compare(*this, a);
    }

    //Filters
    /*! \details Filters the signal using the specified FIR filter. */
    //SignalQ15 filter(const FirFilterQ15 & filter) const;


    /*! \details Filters the signal using the specified FIR filter.
     *
     * @param output A reference to the destination value of the filtered signal.
     * @param filter The filter to use
     *
     */
    //void filter(SignalQ15 & output, const FirFilterQ15 & filter) const;

    SignalQ15 filter(const BiquadDirectFormOneFilterQ15 & filter) const;
    void filter(SignalQ15 & output, const BiquadDirectFormOneFilterQ15 & filter) const;

    //SignalQ15 filter(const FirDecimateFilterQ15 & filter);
    //void filter(SignalQ15 & output, const FirDecimateFilterQ15 & filter);

private:
    friend class BiquadDirectFormOneFilterQ15;

};

/*! \brief Fixed point q1.31 data signal
 * \details The SignalQ31 class stores
 * a real q1.31 data signal.
 *
 *
 */
class SignalQ31 : public SignalData<q31_t> {
public:

    /*! \details Constructs a signal with uninitialized
     * values.
     *
     * @param count The number of data points
     *
     */
    SignalQ31(int count) : SignalData<q31_t>(count){}

    /*! \details Contructs an empty signal. */
    SignalQ31(){}

    //mean
    /*! \details Calculates and returns the mean value of the signal.
     *
     * This method utilizes the arm_mean_q31() function.
     *
     * \code
     * SignalQ31 data(16);
     * data.fill(100);
     * printf("Mean is %ld\n", data.mean());
     * \endcode
     */
    q31_t mean() const {
        q31_t result;
        arm_dsp_api_q31()->mean((q31_t*)data_const(), count(), &result);
        return result;
    }

    q63_t power() const {
        q63_t result;
        arm_dsp_api_q31()->power((q31_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the variance value of the signal.
     *
     * This method utilizes the arm_var_q31() function.
     *
     *
     */
    q31_t variance() const {
        q31_t result;
        arm_dsp_api_q31()->var((q31_t*)data_const(), count(), &result);
        return result;
    }

    q31_t rms() const {
        q31_t result;
        arm_dsp_api_q31()->rms((q31_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the standard deviation value of the signal.
     *
     * This method utilizes the arm_std_q31() function.
     */
    q31_t std() const {
        q31_t result;
        arm_dsp_api_q31()->std((q31_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the minimum value of the signal.
     *
     * This method utilizes the arm_min_q31() function.
     */
    q31_t min() const {
        q31_t result;
        u32 idx;
        arm_dsp_api_q31()->min((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the minimum value of the signal.
     *
     * @param idx A reference to the index value of the minimum. This parameter
     * will be written with the location of the minimum value.
     *
     * This method utilizes the arm_min_q31() function.
     */
    q31_t min(u32 & idx) const {
        q31_t result;
        arm_dsp_api_q31()->min((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the maximum value of the signal.
     *
     * This method utilizes the arm_max_q31() function.
     */
    q31_t max() const {
        q31_t result;
        u32 idx;
        arm_dsp_api_q31()->max((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the maximum value of the signal.
     *
     * @param idx A reference to the index value of the maximum. This parameter
     * will be written with the location of the maximum value.
     *
     * This method utilizes the arm_max_q31() function.
     */
    q31_t max(u32 & idx) const {
        q31_t result;
        arm_dsp_api_q31()->max((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }


    //abs
    /*!
     * \details Calculates the absolute value of each value in the signal.
     * \return A new signal containing the absolute value of this signal.
     *
     * This method will allocate a new signal and return it.
     *
     */
    SignalQ31 abs() const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->abs(
                    (q31_t *)data_const(),
                    ret.vector_data(),
                    count());
        ret.set_transfer_ownership();
        return ret;
    }
    /*!
     * \details Calculates the absolute value of each value in the signal.
     * \param output A reference to the destination signal
     * \return A new signal containing the absolute value of this signal.
     *
     */
    void abs(SignalQ31 & output) const {
        arm_dsp_api_q31()->abs(
                    (q31_t *)data_const(),
                    output.vector_data(),
                    count());
    }

    //add
    SignalQ31 operator + (const SignalQ31 & a) const{
        SignalQ31 ret(count());
        arm_dsp_api_q31()->add((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalQ31 & operator += (const SignalQ31 & a){
        arm_dsp_api_q31()->add((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), vector_data(), count());
        return *this;
    }

    //dot prod
    /*!
     * \brief Calculates and returns the dot product of this signal and the the parameter passed.
     *
     * \param a Used as the second signal used to calculate the dot product.
     * \return Returns the dot product of \a a and this signal.
     */
    q63_t dot_product(const SignalQ31 & a) const {
        q63_t result;
        arm_dsp_api_q31()->dot_prod((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), count(), &result);
        return result;
    }

    //mult
    /*!
     * \details Calculates element-by-element multiplication.
     *
     * \param a The second operand of the multiply operation
     * \return A new signal containing the product of \a a and this signal.
     *
     */
    SignalQ31 operator * (const SignalQ31 & a) const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->mult((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }


    /*! \details Adds an offset to each element in the signal. */
    SignalQ31 offset(q31_t offset_value) const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->offset((q31_t*)vector_data_const(), offset_value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    /*! \details Returns a new signal that a negated copy of this signal.
     *
     * This method uses dynamic memory allocation.
     *
     */
    SignalQ31 negate() const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->negate((q31_t*)vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    /*! \details Assigns the negated value of this signal to \a output. */
    void negate(SignalQ31 & output) const {
        arm_dsp_api_q31()->negate((q31_t*)vector_data_const(), output.vector_data(), count());
    }


    SignalQ31 convolve(const SignalQ31 & a) const {
        SignalQ31 ret(count() + a.count() - 1);
        arm_dsp_api_q31()->conv_fast((q31_t*)vector_data_const(), count(), (q31_t*)a.vector_data_const(), a.count(), ret.vector_data());
        ret.set_transfer_ownership();
        return ret;
    }

    void convolve(SignalQ31 & output, const SignalQ31 & a) const {
        //check output length?
        arm_dsp_api_q31()->conv_fast((q31_t*)vector_data_const(), count(), (q31_t*)a.vector_data_const(), a.count(), output.vector_data());
    }


    SignalQ31 scale(q31_t scale_fraction, int8_t shift = 0){
        SignalQ31 ret(count());
        arm_dsp_api_q31()->scale((q31_t*)vector_data_const(), scale_fraction, shift, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    void scale(SignalQ31 & output, q31_t scale_fraction, int8_t shift = 0){
        arm_dsp_api_q31()->scale((q31_t*)vector_data_const(), scale_fraction, shift, output.vector_data(), count());
    }

    //shift
    SignalQ31 shift(int8_t value){
        SignalQ31 ret(count());
        arm_dsp_api_q31()->shift((q31_t*)vector_data_const(), value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    void shift(SignalQ31 & output, int8_t value){
        arm_dsp_api_q31()->shift((q31_t*)vector_data_const(), value, output.vector_data(), count());
    }

    SignalQ31 operator * (q31_t value){
        return scale(value);
    }

    //subtract
    SignalQ31 operator - (const SignalQ31 & a) const{
        SignalQ31 ret(count());
        arm_dsp_api_q31()->sub((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalQ31 & operator -= (const SignalQ31 & a){
        arm_dsp_api_q31()->sub((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), vector_data(), count());
        return *this;
    }

    bool operator == (const SignalQ31 & a) const {
        return compare(*this, a);
    }

    bool operator != (const SignalQ31 & a) const {
        return !compare(*this, a);
    }

    //Filters
    /*! \details Filters the signal using the specified FIR filter. */
    SignalQ31 filter(const FirFilterQ31 & filter) const;


    /*! \details Filters the signal using the specified FIR filter.
     *
     * @param output A reference to the destination value of the filtered signal.
     * @param filter The filter to use
     *
     */
    void filter(SignalQ31 & output, const FirFilterQ31 & filter) const;

    SignalQ31 filter(const BiquadDirectFormOneFilterQ31 & filter) const;
    void filter(SignalQ31 & output, const BiquadDirectFormOneFilterQ31 & filter) const;

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
    friend class FirFilterQ31;
    friend class BiquadDirectFormOneFilterQ31;
    friend class FirDecimateFilterQ31;


};

class SignalComplexQ31 : public SignalData<ComplexQ31> {
public:

    SignalComplexQ31(){}
    SignalComplexQ31(u32 count) : SignalData<ComplexQ31>(count){}

    static SignalComplexQ31 create_transform_source(const ComplexFftQ31 & fft);
    static SignalComplexQ31 create_transform_dest(const ComplexFftQ31 & fft);

    SignalComplexQ31 transform(const RealFftQ31 & fft, bool is_inverse = false);
    void transform(SignalComplexQ31 & output, const RealFftQ31 & fft, bool is_inverse = false);

    void transform(const ComplexFftQ31 & fft, bool is_inverse = false, bool is_bit_reversal = false);


protected:
    const q31_t * vector_data_const() const { return (const q31_t*)SignalData<ComplexQ31>::vector_data_const(); }
    q31_t * vector_data(){ return (q31_t*)SignalData<ComplexQ31>::vector_data(); }


private:
    friend class RealFftQ31;
    friend class ComplexFftQ31;
};

class SignalF32 : public SignalData<float32_t> {
public:
    SignalF32(int count) : SignalData<float32_t>(count){}
    SignalF32(){}

    /*! \details Calculates and returns the mean value of the signal.
     *
     * This method utilizes the arm_mean_f32() function.
     *
     * \code
     * SignalF32 data(16);
     * data.fill(100);
     * printf("Mean is %ld\n", data.mean());
     * \endcode
     */
    float32_t mean() const {
        float32_t result;
        arm_dsp_api_f32()->mean((float32_t*)data_const(), count(), &result);
        return result;
    }

    float32_t power() const {
        float32_t result;
        arm_dsp_api_f32()->power((float32_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the variance value of the signal.
     *
     * This method utilizes the arm_var_f32() function.
     *
     *
     */
    float32_t variance() const {
        float32_t result;
        arm_dsp_api_f32()->var((float32_t*)data_const(), count(), &result);
        return result;
    }

    float32_t rms() const {
        float32_t result;
        arm_dsp_api_f32()->rms((float32_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the standard deviation value of the signal.
     *
     * This method utilizes the arm_std_f32() function.
     */
    float32_t std() const {
        float32_t result;
        arm_dsp_api_f32()->std((float32_t*)data_const(), count(), &result);
        return result;
    }

    /*! \details Calculates and returns the minimum value of the signal.
     *
     * This method utilizes the arm_min_f32() function.
     */
    float32_t min() const {
        float32_t result;
        u32 idx;
        arm_dsp_api_f32()->min((float32_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the minimum value of the signal.
     *
     * @param idx A reference to the index value of the minimum. This parameter
     * will be written with the location of the minimum value.
     *
     * This method utilizes the arm_min_f32() function.
     */
    float32_t min(u32 & idx) const {
        float32_t result;
        arm_dsp_api_f32()->min((float32_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the maximum value of the signal.
     *
     * This method utilizes the arm_max_f32() function.
     */
    float32_t max() const {
        float32_t result;
        u32 idx;
        arm_dsp_api_f32()->max((float32_t*)data_const(), count(), &result, &idx);
        return result;
    }

    /*! \details Calculates and returns the maximum value of the signal.
     *
     * @param idx A reference to the index value of the maximum. This parameter
     * will be written with the location of the maximum value.
     *
     * This method utilizes the arm_max_f32() function.
     */
    float32_t max(u32 & idx) const {
        float32_t result;
        arm_dsp_api_f32()->max((float32_t*)data_const(), count(), &result, &idx);
        return result;
    }


    //abs
    /*!
     * \details Calculates the absolute value of each value in the signal.
     * \return A new signal containing the absolute value of this signal.
     *
     * This method will allocate a new signal and return it.
     *
     */
    SignalF32 abs() const {
        SignalF32 ret(count());
        arm_dsp_api_f32()->abs(
                    (float32_t *)data_const(),
                    ret.vector_data(),
                    count());
        ret.set_transfer_ownership();
        return ret;
    }
    /*!
     * \details Calculates the absolute value of each value in the signal.
     * \param output A reference to the destination signal
     * \return A new signal containing the absolute value of this signal.
     *
     */
    void abs(SignalF32 & output) const {
        arm_dsp_api_f32()->abs(
                    (float32_t *)data_const(),
                    output.vector_data(),
                    count());
    }

    //add
    SignalF32 operator + (const SignalF32 & a) const{
        SignalF32 ret(count());
        arm_dsp_api_f32()->add((float32_t*)vector_data_const(), (float32_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalF32 & operator += (const SignalF32 & a){
        arm_dsp_api_f32()->add((float32_t*)vector_data_const(), (float32_t*)a.vector_data_const(), vector_data(), count());
        return *this;
    }

    //dot prod
    /*!
     * \brief Calculates and returns the dot product of this signal and the the parameter passed.
     *
     * \param a Used as the second signal used to calculate the dot product.
     * \return Returns the dot product of \a a and this signal.
     */
    float32_t dot_product(const SignalF32 & a) const {
        float32_t result;
        arm_dsp_api_f32()->dot_prod((float32_t*)vector_data_const(), (float32_t*)a.vector_data_const(), count(), &result);
        return result;
    }

    //mult
    /*!
     * \details Calculates element-by-element multiplication.
     *
     * \param a The second operand of the multiply operation
     * \return A new signal containing the product of \a a and this signal.
     *
     */
    SignalF32 operator * (const SignalF32 & a) const {
        SignalF32 ret(count());
        arm_dsp_api_f32()->mult((float32_t*)vector_data_const(), (float32_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }


    /*! \details Adds an offset to each element in the signal. */
    SignalF32 offset(float32_t offset_value) const {
        SignalF32 ret(count());
        arm_dsp_api_f32()->offset((float32_t*)vector_data_const(), offset_value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    /*! \details Returns a new signal that a negated copy of this signal.
     *
     * This method uses dynamic memory allocation.
     *
     */
    SignalF32 negate() const {
        SignalF32 ret(count());
        arm_dsp_api_f32()->negate((float32_t*)vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    /*! \details Assigns the negated value of this signal to \a output. */
    void negate(SignalF32 & output) const {
        arm_dsp_api_f32()->negate((float32_t*)vector_data_const(), output.vector_data(), count());
    }


    SignalF32 convolve(const SignalF32 & a) const {
        SignalF32 ret(count() + a.count() - 1);
        arm_dsp_api_f32()->conv((float32_t*)vector_data_const(), count(), (float32_t*)a.vector_data_const(), a.count(), ret.vector_data());
        ret.set_transfer_ownership();
        return ret;
    }

    void convolve(SignalF32 & output, const SignalF32 & a) const {
        //check output length?
        arm_dsp_api_f32()->conv((float32_t*)vector_data_const(), count(), (float32_t*)a.vector_data_const(), a.count(), output.vector_data());
    }


    SignalF32 scale(float32_t scale_fraction, int8_t shift = 0){
        SignalF32 ret(count());
        arm_dsp_api_f32()->scale((float32_t*)vector_data_const(), scale_fraction, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    void scale(SignalF32 & output, float32_t scale_fraction, int8_t shift = 0){
        arm_dsp_api_f32()->scale((float32_t*)vector_data_const(), scale_fraction, output.vector_data(), count());
    }

    //shift
    SignalF32 shift(int8_t value){
        SignalF32 ret(count());
        //arm_dsp_api_f32()->shift((float32_t*)vector_data_const(), value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    void shift(SignalF32 & output, int8_t value){
        //arm_dsp_api_f32()->shift((float32_t*)vector_data_const(), value, output.vector_data(), count());
    }

    SignalF32 operator * (float32_t value){
        return scale(value);
    }

    //subtract
    SignalF32 operator - (const SignalF32 & a) const{
        SignalF32 ret(count());
        arm_dsp_api_f32()->sub((float32_t*)vector_data_const(), (float32_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalF32 & operator -= (const SignalF32 & a){
        arm_dsp_api_f32()->sub((float32_t*)vector_data_const(), (float32_t*)a.vector_data_const(), vector_data(), count());
        return *this;
    }

    bool operator == (const SignalF32 & a) const {
        return compare(*this, a);
    }

    bool operator != (const SignalF32 & a) const {
        return !compare(*this, a);
    }

    //Filters
    /*! \details Filters the signal using the specified FIR filter. */
    //SignalF32 filter(const FirFilterF32 & filter) const;


    /*! \details Filters the signal using the specified FIR filter.
     *
     * @param output A reference to the destination value of the filtered signal.
     * @param filter The filter to use
     *
     */
    //void filter(SignalF32 & output, const FirFilterF32 & filter) const;

    SignalF32 filter(const BiquadDirectFormOneFilterF32 & filter) const;
    void filter(SignalF32 & output, const BiquadDirectFormOneFilterF32 & filter) const;


private:
    friend class BiquadDirectFormOneFilterF32;


};

typedef SignalF32 SignalFloat32;

}

#endif // DSP_SIGNAL_DATA_HPP
