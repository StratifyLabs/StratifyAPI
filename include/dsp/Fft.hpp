#ifndef DSP_FFT_HPP
#define DSP_FFT_HPP

#include "SignalData.hpp"
#include "../api/DspObject.hpp"

namespace dsp {

template<typename type_value> class Fft : public api::DspWorkObject {
public:
    Fft();

    virtual int init(const SignalData<type_value> & data) = 0;
    virtual void fft(SignalData<type_value> & data) = 0;
    virtual void ifft(SignalData<type_value> & data) = 0;

    void set_bit_reversal(bool value = true){
        m_is_bit_reversal = value;
    }

    bool is_bit_reversal() const {
        return m_is_bit_reversal;
    }

private:
    bool m_is_bit_reversal;

};

class CfftQ15 : public Fft<q15_t> {
public:
    CfftQ15();

    int init(const SignalQ15 & data){
        //cfft is initialized using a hack - RFFT init will grab the data needed
        arm_rfft_instance_q15 rfft_instance;
        if( arm_dsp_api_q15()->rfft_init(&rfft_instance, data.count(), 0, 0) == ARM_MATH_SUCCESS){
            m_instance = rfft_instance.pCfft;
            return 0;
        }
        return -1;
    }

    void fft(SignalQ15 & data){
        arm_dsp_api_q15()->cfft(m_instance, data.data(), 0, is_bit_reversal());
    }

    void ifft(SignalQ15 & data){
        arm_dsp_api_q15()->cfft(m_instance, data.data(), 1, is_bit_reversal());
    }

private:
    const arm_cfft_instance_q15 * m_instance;
};

class CfftQ31 : public Fft<q31_t> {
public:
    CfftQ31();

    int init(const SignalQ31 & data){
        //cfft is initialized using a hack - RFFT init will grab the data needed
        arm_rfft_instance_q31 rfft_instance;
        if( arm_dsp_api_q31()->rfft_init(&rfft_instance, data.count(), 0, 0) == ARM_MATH_SUCCESS){
            m_instance = rfft_instance.pCfft;
            return 0;
        }
        return -1;
    }

    void fft(SignalQ31 & data){
        arm_dsp_api_q31()->cfft(m_instance, data.data(), 0, is_bit_reversal());
    }

    void ifft(SignalQ31 & data){
        arm_dsp_api_q31()->cfft(m_instance, data.data(), 1, is_bit_reversal());
    }

private:
    const arm_cfft_instance_q31 * m_instance;
};

class CfftF32 : public Fft<float32_t> {
public:
    CfftF32();

    int init(const SignalF32 & data){
        //cfft is initialized using a hack - RFFT init will grab the data needed
        arm_rfft_fast_instance_f32 rfft_instance;
        if( arm_dsp_api_f32()->rfft_fast_init(&rfft_instance, data.count()) == ARM_MATH_SUCCESS){
            m_instance = rfft_instance.Sint;
            return 0;
        }
        return -1;
    }

    void fft(SignalF32 & data){
        arm_dsp_api_f32()->cfft(&m_instance, data.data(), 0, is_bit_reversal());
    }

    void ifft(SignalF32 & data){
        arm_dsp_api_f32()->cfft(&m_instance, data.data(), 1, is_bit_reversal());
    }

private:
    arm_cfft_instance_f32 m_instance;
};

}

#endif // CFFT_HPP
