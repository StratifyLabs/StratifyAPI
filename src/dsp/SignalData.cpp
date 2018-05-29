#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

SignalComplexQ31 SignalComplexQ31::transform(const ComplexFftQ31 & fft, bool is_inverse, bool is_bit_reversal){
    SignalComplexQ31 ret(count()*2);
    arm_dsp_api_q31()->cfft(fft.instance(), vector_data(), is_inverse, is_bit_reversal);
    ret.set_transfer_ownership();
    return ret;
}

SignalComplexQ31 SignalComplexQ31::transform(const RealFftQ31 & fft, bool is_inverse){
    RealFftQ31 rfft = fft;
    SignalComplexQ31 ret(count()*2);
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q31()->rfft(rfft.instance(), vector_data(), ret.vector_data());
    ret.set_transfer_ownership();
    return ret;
}

SignalQ31 SignalQ31::filter(const FirFilterQ31 & filter){
    SignalQ31 ret(count());
    arm_dsp_api_q31()->fir_fast(filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

SignalQ31 SignalQ31::filter(const BiquadCascadeDf1FilterQ31 & filter){
    SignalQ31 ret(count());
    arm_dsp_api_q31()->biquad_cascade_df1_fast(filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}


