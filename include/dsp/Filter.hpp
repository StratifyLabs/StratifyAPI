#ifndef DSP_FILTER_HPP
#define DSP_FILTER_HPP

#include "../api/DspObject.hpp"
#include "SignalData.hpp"

namespace dsp {

template<typename T> class Filter : public api::DspWorkObject {
public:

    const T * instance() const { return &m_instance; }

protected:
    T * instance(){ return &m_instance; }

private:
    T m_instance;
};


class FirFilterQ31 : public Filter<arm_fir_instance_q31> {
public:
    FirFilterQ31(const SignalQ31 & coefficients, u32 n_samples);

private:
    SignalQ31 m_state;

};

class BiquadCascadeDf1FilterQ31 : public Filter<arm_biquad_casd_df1_inst_q31> {
public:

    BiquadCascadeDf1FilterQ31(const SignalQ31 & coefficients, s8 post_shift);

private:
    SignalQ31 m_state;
};


}

#endif // FILTER_HPP
