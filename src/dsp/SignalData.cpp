#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

SignalComplexQ31 SignalComplexQ31::create_transform_source(const ComplexFftQ31 & fft){
    SignalComplexQ31 ret( fft.samples() );
    ret.set_transfer_ownership();
    return ret;
}

SignalComplexQ31 SignalComplexQ31::create_transform_dest(const ComplexFftQ31 & fft){
    SignalComplexQ31 ret( fft.samples()*2 );
    ret.set_transfer_ownership();
    return ret;
}

void SignalComplexQ31::transform(const ComplexFftQ31 & fft, bool is_inverse, bool is_bit_reversal){
    arm_dsp_api_q31()->cfft(fft.instance(), vector_data(), is_inverse, is_bit_reversal);
}

void SignalComplexQ31::transform(SignalComplexQ31 & output, const RealFftQ31 & fft, bool is_inverse){
    RealFftQ31 rfft = fft;
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q31()->rfft(rfft.instance(), vector_data(), output.vector_data());
}

SignalComplexQ31 SignalComplexQ31::transform(const RealFftQ31 & fft, bool is_inverse){
    RealFftQ31 rfft = fft;
    SignalComplexQ31 ret(count()*2);
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q31()->rfft(rfft.instance(), vector_data(), ret.vector_data());
    ret.set_transfer_ownership();
    return ret;
}

SignalQ31 SignalQ31::filter(const FirFilterQ31 & filter) const {
    SignalQ31 ret(count());
    arm_dsp_api_q31()->fir_fast(filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ31::filter(SignalQ31 & output, const FirFilterQ31 & filter) const {
    arm_dsp_api_q31()->fir_fast(filter.instance(), (q31_t*)vector_data_const(), output.vector_data(), count());
}

SignalQ15 SignalQ15::filter(const BiquadDirectFormOneFilterQ15 & filter) const {
    SignalQ15 ret(count());
    arm_dsp_api_q15()->biquad_cascade_df1_fast(filter.instance(), (q15_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ15::filter(SignalQ15 & output, const BiquadDirectFormOneFilterQ15 & filter) const {
    arm_dsp_api_q15()->biquad_cascade_df1_fast(filter.instance(), (q15_t*)vector_data_const(), output.vector_data(), count());
}

SignalQ31 SignalQ31::filter(const BiquadDirectFormOneFilterQ31 & filter) const {
    SignalQ31 ret(count());
    arm_dsp_api_q31()->biquad_cascade_df1_fast(filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ31::filter(SignalQ31 & output, const BiquadDirectFormOneFilterQ31 & filter) const {
    arm_dsp_api_q31()->biquad_cascade_df1_fast(filter.instance(), (q31_t*)vector_data_const(), output.vector_data(), count());
}

SignalQ31 SignalQ31::filter(const FirDecimateFilterQ31 & filter){
    SignalQ31 ret(count());
    arm_dsp_api_q31()->fir_decimate_fast((arm_fir_decimate_instance_q31*)filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ31::filter(SignalQ31 & output, const FirDecimateFilterQ31 & filter){
    arm_dsp_api_q31()->fir_decimate_fast((arm_fir_decimate_instance_q31*)filter.instance(), (q31_t*)vector_data_const(), output.vector_data(), count());
}

SignalQ31 SignalQ31::create_sin_wave(u32 wave_frequency, u32 sampling_frequency, u32 samples, q31_t phase){
    SignalQ31 ret(samples);
    u32 i;
    q31_t theta = phase; //theta 0 to max is 0 to 2*pi
    q31_t theta_step = (q63_t)wave_frequency * INT_FAST32_MAX / sampling_frequency;

    //fsamp = 16K and fmax = 8k - theta step is pi or (1/2)
    //theta step is f / fsamp
    for(i=0; i < samples; i++){
        ret[i] = arm_dsp_api_q31()->sin(theta);
        theta += theta_step;
    }

    ret.set_transfer_ownership();
    return ret;
}

