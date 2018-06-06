#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

void SignalComplexQ15::transform(const FftComplexQ15 & fft, bool is_inverse, bool is_bit_reversal){
    arm_dsp_api_q15()->cfft(fft.instance(), vector_data(), is_inverse, is_bit_reversal);
}

void SignalComplexQ15::transform(SignalComplexQ15 & output, const FftRealQ15 & fft, bool is_inverse){
    FftRealQ15 rfft = fft;
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q15()->rfft(rfft.instance(), vector_data(), output.vector_data());
}

SignalComplexQ15 SignalComplexQ15::transform(const FftRealQ15 & fft, bool is_inverse){
    FftRealQ15 rfft = fft;
    int samples = count();
    if( !is_inverse ){ samples *= 2; }
    SignalComplexQ15 ret(samples);
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q15()->rfft(rfft.instance(), vector_data(), ret.vector_data());
    ret.set_transfer_ownership();
    return ret;
}

void SignalComplexQ31::transform(const FftComplexQ31 & fft, bool is_inverse, bool is_bit_reversal){
    arm_dsp_api_q31()->cfft(fft.instance(), vector_data(), is_inverse, is_bit_reversal);
}

void SignalComplexQ31::transform(SignalComplexQ31 & output, const FftRealQ31 & fft, bool is_inverse){
    FftRealQ31 rfft = fft;
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q31()->rfft(rfft.instance(), vector_data(), output.vector_data());
}

SignalComplexQ31 SignalComplexQ31::transform(const FftRealQ31 & fft, bool is_inverse){
    FftRealQ31 rfft = fft;
    int samples = count();
    if( !is_inverse ){ samples *= 2; }
    SignalComplexQ31 ret(samples);
    rfft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_q31()->rfft(rfft.instance(), vector_data(), ret.vector_data());
    ret.set_transfer_ownership();
    return ret;
}

void SignalComplexF32::transform(const FftComplexF32 & fft, bool is_inverse, bool is_bit_reversal){
    arm_dsp_api_f32()->cfft(fft.instance(), vector_data(), is_inverse, is_bit_reversal);
}

void SignalComplexF32::transform(SignalComplexF32 & output, const FftRealF32 & fft, bool is_inverse){
    arm_dsp_api_f32()->rfft_fast((arm_rfft_fast_instance_f32*)fft.instance(), vector_data(), output.vector_data(), is_inverse);
}

SignalComplexF32 SignalComplexF32::transform(const FftRealF32 & fft, bool is_inverse){
    FftRealF32 rfft = fft;
    int samples = count();
    if( !is_inverse ){ samples *= 2; }
    SignalComplexF32 ret(samples);
    arm_dsp_api_f32()->rfft_fast(rfft.instance(), vector_data(), ret.vector_data(), is_inverse);
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

SignalQ15 SignalQ15::filter(const BiquadFilterQ15 & filter) const {
    SignalQ15 ret(count());
    arm_dsp_api_q15()->biquad_cascade_df1_fast(filter.instance(), (q15_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ15::filter(SignalQ15 & output, const BiquadFilterQ15 & filter) const {
    arm_dsp_api_q15()->biquad_cascade_df1_fast(filter.instance(), (q15_t*)vector_data_const(), output.vector_data(), count());
}

SignalQ31 SignalQ31::filter(const BiquadFilterQ31 & filter) const {
    SignalQ31 ret(count());
    arm_dsp_api_q31()->biquad_cascade_df1_fast(filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ31::filter(SignalQ31 & output, const BiquadFilterQ31 & filter) const {
    arm_dsp_api_q31()->biquad_cascade_df1_fast(filter.instance(), (q31_t*)vector_data_const(), output.vector_data(), count());
}

SignalF32 SignalF32::filter(const BiquadFilterF32 & filter) const {
    SignalF32 ret(count());
    arm_dsp_api_f32()->biquad_cascade_df1(filter.instance(), (float32_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalF32::filter(SignalF32 & output, const BiquadFilterF32 & filter) const {
    arm_dsp_api_f32()->biquad_cascade_df1(filter.instance(), (float32_t*)vector_data_const(), output.vector_data(), count());
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

