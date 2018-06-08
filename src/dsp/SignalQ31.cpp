#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

#define IS_FLOAT 0
#define native_type q31_t
#define arm_dsp_api_function() arm_dsp_api_q31()
#define SignalDataType SignalDataQ31
#define SignalType SignalQ31
#define SignalComplexType SignalComplexQ31
#define FftRealType FftRealQ31
#define FftComplexType FftComplexQ31
#define BiquadFilterType BiquadFilterQ31
#define big_type q63_t

#include "SignalDataGeneric.h"

SignalQ31 SignalQ31::filter(const FirFilterQ31 & filter) const {
    SignalQ31 ret(count());
    arm_dsp_api_q31()->fir_fast(filter.instance(), (q31_t*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalQ31::filter(SignalQ31 & output, const FirFilterQ31 & filter) const {
    arm_dsp_api_q31()->fir_fast(filter.instance(), (q31_t*)vector_data_const(), output.vector_data(), count());
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

