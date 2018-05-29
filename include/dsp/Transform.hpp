#ifndef DSP_TRANSFORM_HPP
#define DSP_TRANSFORM_HPP

#include "../api/DspObject.hpp"

namespace dsp {

class SignalQ15;
class SignalQ31;
class SignalF32;

template<typename T> class Fft : public api::DspWorkObject {
public:
    const T * instance() const { return &m_instance; }
    T * instance(){ return &m_instance; }

private:
    T m_instance;
};

class ComplexFftQ15 : public api::DspWorkObject {
public:
    ComplexFftQ15(u32 n_samples);
    const arm_cfft_instance_q15 * instance() const { return m_instance; }

private:
    const arm_cfft_instance_q15 * m_instance;
};

class ComplexFftQ31 : public api::DspWorkObject {
public:
    ComplexFftQ31(u32 n_samples);

    const arm_cfft_instance_q31 * instance() const { return m_instance; }

private:
    const arm_cfft_instance_q31 * m_instance;
};

class ComplexFftF32 : public api::DspWorkObject {
public:
    ComplexFftF32(u32 n_samples);

    arm_cfft_instance_f32 * instance(){ return &m_instance; }
    const arm_cfft_instance_f32 * instance() const { return &m_instance; }

private:
    arm_cfft_instance_f32 m_instance;
};

class RealFftQ15 : public Fft<arm_rfft_instance_q15> {
public:
    RealFftQ15(u32 n_samples);

private:
};

class RealFftQ31 : public Fft<arm_rfft_instance_q31> {
public:
    RealFftQ31(u32 n_samples);

private:

};

class RealFftF32 : public Fft<arm_rfft_fast_instance_f32> {
public:
    RealFftF32(u32 n_samples);

private:

};

//Add DCT classes

}

#endif // DSP_TRANSFORM_HPP
