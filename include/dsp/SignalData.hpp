#ifndef DSP_SIGNAL_DATA_HPP
#define DSP_SIGNAL_DATA_HPP

#include "../api/DspObject.hpp"
#include "../var/Vector.hpp"

namespace dsp {

class CfftQ15;
class RfftQ15;
class CfftQ31;
class RfftQ31;
class CfftF32;
class RfftF32;

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
    SignalQ7(int count) : SignalData(count){}

};

class SignalQ15 : public SignalData<q15_t> {
public:
    SignalQ15(int count) : SignalData(count){}
    SignalQ15(){}

};

class SignalQ31 : public SignalData<q31_t> {
public:
    SignalQ31(int count) : SignalData(count){}
    SignalQ31(){}

    //mean
    q31_t mean() const {
        q31_t result;
        arm_dsp_api_q31()->mean((q31_t*)data_const(), count(), &result);
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

    q31_t min(u32 & idx) const {
        q31_t result;
        arm_dsp_api_q31()->min((q31_t*)data_const(), count(), &result, &idx);
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

    //mult

    //negate

    //offset

    //scale

    //shift

    //subtract


    //Transforms
    void fft(const CfftQ31 & fft, bool is_bit_reversal = false);
    void ifft(const CfftQ31 & fft, bool is_bit_reversal = false);
    SignalQ31 fft(const RfftQ31 & fft);
    SignalQ31 ifft(const RfftQ31 & fft);
};

class SignalF32 : public SignalData<float32_t> {
public:
    SignalF32(int count) : SignalData(count){}
    SignalF32(){}

};

typedef SignalF32 SignalFloat32;

}

#endif // DSP_SIGNAL_DATA_HPP
