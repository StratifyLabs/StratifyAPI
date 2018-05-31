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
class BiquadCascadeDf1FilterQ31;
class FirDecimateFilterQ31;

class ComplexQ31 {
public:
    q31_t & real(){ return m_value[0]; }
    q31_t & complex(){ return m_value[1]; }

    const q31_t & real() const { return m_value[0]; }
    const q31_t & complex() const { return m_value[1]; }

    ComplexQ31 operator + (const ComplexQ31 & a) const {
        ComplexQ31 value;
        value.real() = real() + a.real();
        value.complex() = complex() + a.complex();
        return value;
    }

    ComplexQ31 operator - (const ComplexQ31 & a) const {
        ComplexQ31 value;
        value.real() = real() - a.real();
        value.complex() = complex() - a.complex();
        return value;
    }

    bool operator == (const ComplexQ31 & a) const {
        return (real() == a.real()) && (complex() == a.complex());
    }

    bool operator != (const ComplexQ31 & a) const {
        return (real() != a.real()) && (complex() != a.complex());
    }

private:
    q31_t m_value[2];
};

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

};

class SignalQ31 : public SignalData<q31_t> {
public:
    SignalQ31(int count) : SignalData<q31_t>(count){}
    SignalQ31(){}

    //mean
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

    q31_t std() const {
        q31_t result;
        arm_dsp_api_q31()->std((q31_t*)data_const(), count(), &result);
        return result;
    }

    q31_t min() const {
        q31_t result;
        u32 idx;
        arm_dsp_api_q31()->min((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }

    q31_t min(u32 & idx) const {
        q31_t result;
        arm_dsp_api_q31()->min((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }

    q31_t max() const {
        q31_t result;
        u32 idx;
        arm_dsp_api_q31()->max((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }


    q31_t max(u32 & idx) const {
        q31_t result;
        arm_dsp_api_q31()->max((q31_t*)data_const(), count(), &result, &idx);
        return result;
    }


    //abs
    SignalQ31 abs() const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->abs(
                    (q31_t *)data_const(),
                    ret.vector_data(),
                    count());
        ret.set_transfer_ownership();
        return ret;
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
    q63_t dot_product(const SignalQ31 & a) const {
        q63_t result;
        arm_dsp_api_q31()->dot_prod((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), count(), &result);
        return result;
    }

    //mult
    SignalQ31 operator * (const SignalQ31 & a) const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->mult((q31_t*)vector_data_const(), (q31_t*)a.vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    //negate

    //offset
    SignalQ31 offset(q31_t offset_value) const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->offset((q31_t*)vector_data_const(), offset_value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalQ31 negate() const {
        SignalQ31 ret(count());
        arm_dsp_api_q31()->negate((q31_t*)vector_data_const(), ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    SignalQ31 convolve(const SignalQ31 & a) const {
        SignalQ31 ret(count() + a.count() - 1);
        arm_dsp_api_q31()->conv_fast((q31_t*)vector_data_const(), count(), (q31_t*)a.vector_data_const(), a.count(), ret.vector_data());
        ret.set_transfer_ownership();
        return ret;
    }

    //scale
    SignalQ31 scale(q31_t scale_fraction, int8_t shift = 0){
        SignalQ31 ret(count());
        arm_dsp_api_q31()->scale((q31_t*)vector_data_const(), scale_fraction, shift, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
    }

    //shift
    SignalQ31 shift(int8_t value){
        SignalQ31 ret(count());
        arm_dsp_api_q31()->shift((q31_t*)vector_data_const(), value, ret.vector_data(), count());
        ret.set_transfer_ownership();
        return ret;
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
    SignalQ31 filter(const FirFilterQ31 & filter) const;
    void filter(SignalQ31 & output, const FirFilterQ31 & filter) const;

    SignalQ31 filter(const BiquadCascadeDf1FilterQ31 & filter) const;
    void filter(SignalQ31 & output, const BiquadCascadeDf1FilterQ31 & filter) const;

    SignalQ31 filter(const FirDecimateFilterQ31 & filter);
    void filter(SignalQ31 & output, const FirDecimateFilterQ31 & filter);
    //void filter(const FirInterpolateFilterQ31 & filter);
    //void filter(const FirLatticeFilterQ31 & filter);
    //void filter(const IirLatticeFilterQ31 & filter);
    //void filter(const BiquadCascadeFilterQ31 & filter);
    //void filter(const LmsFilterQ31 & filter);
    //void filter(const LmsNormalFilterQ31 & filter);

private:
    friend class FirFilterQ31;
    friend class BiquadCascadeDf1FilterQ31;
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

};

typedef SignalF32 SignalFloat32;

}

#endif // DSP_SIGNAL_DATA_HPP
