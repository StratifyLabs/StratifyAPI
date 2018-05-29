#ifndef DSP_TRANSFORM_HPP
#define DSP_TRANSFORM_HPP

#include "../api/DspObject.hpp"

namespace dsp {

class SignalQ15;
class SignalQ31;
class SignalF32;

class CfftQ15 : public api::DspWorkObject {
public:
    CfftQ15();

    int init(const SignalQ15 & data);
    const arm_cfft_instance_q15 * instance() const { return m_instance; }

private:
    const arm_cfft_instance_q15 * m_instance;
};

class CfftQ31 : public api::DspWorkObject {
public:
    CfftQ31();

    int init(const SignalQ31 & data);

    const arm_cfft_instance_q31 * instance() const { return m_instance; }

private:
    const arm_cfft_instance_q31 * m_instance;
};

class CfftF32 : public api::DspWorkObject {
public:
    CfftF32();

    int init(const SignalF32 & data);

    arm_cfft_instance_f32 * instance(){ return &m_instance; }
    const arm_cfft_instance_f32 * instance() const { return &m_instance; }

private:
    arm_cfft_instance_f32 m_instance;
};

class RfftQ15 : public api::DspWorkObject {
public:
    RfftQ15();

    int init(const SignalQ15 & data);

    arm_rfft_instance_q15 * instance(){ return &m_instance; }
    const arm_rfft_instance_q15 * instance() const { return &m_instance; }

private:
    arm_rfft_instance_q15 m_instance;
};

class RfftQ31 : api::DspWorkObject {
public:
    RfftQ31(){}

    int init(const SignalQ31 & data);

    arm_rfft_instance_q31 * instance(){ return &m_instance; }
    const arm_rfft_instance_q31 * instance() const { return &m_instance; }

private:
    arm_rfft_instance_q31 m_instance;
};

class RfftF32 : public api::DspWorkObject {
public:
    RfftF32();

    int init(const SignalF32 & data);
    arm_rfft_fast_instance_f32 * instance(){ return &m_instance; }
    const arm_rfft_fast_instance_f32 * instance() const { return &m_instance; }

private:
    arm_rfft_fast_instance_f32 m_instance;
};

//Add DCT classes

}

#endif // DSP_TRANSFORM_HPP
