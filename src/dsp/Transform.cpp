#include <errno.h>
#include "dsp/Transform.hpp"
#include "dsp/SignalData.hpp"

using namespace dsp;

ComplexFftQ15::ComplexFftQ15(u32 n_samples){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    arm_rfft_instance_q15 rfft_instance;
    if( arm_dsp_api_q15()->rfft_init(&rfft_instance, n_samples, 0, 0) == ARM_MATH_SUCCESS){
        m_instance = rfft_instance.pCfft;
    }
}

ComplexFftQ31::ComplexFftQ31(u32 n_samples){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    arm_rfft_instance_q31 rfft_instance;
    if( arm_dsp_api_q31()->rfft_init(&rfft_instance, n_samples, 0, 0) == ARM_MATH_SUCCESS){
        m_instance = rfft_instance.pCfft;
    }
}

ComplexFftF32::ComplexFftF32(u32 n_samples){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    arm_rfft_fast_instance_f32 rfft_instance;
    if( arm_dsp_api_f32()->rfft_fast_init(&rfft_instance, n_samples) == ARM_MATH_SUCCESS){
        m_instance = rfft_instance.Sint;
    }
}

RealFftQ15::RealFftQ15(u32 n_samples){
    if( arm_dsp_api_q15()->rfft_init(instance(), n_samples, 0, 0) == ARM_MATH_SUCCESS){
        //return 0;
    }
    //return -1;
}

RealFftQ31::RealFftQ31(u32 n_samples){
    if( arm_dsp_api_q31()->rfft_init(instance(), n_samples, 0, 0) == ARM_MATH_SUCCESS){

    } else {
        set_error_number(EINVAL);
    }
    //return -1;
}

RealFftF32::RealFftF32(u32 n_samples){
    if( arm_dsp_api_f32()->rfft_fast_init(instance(), n_samples) == ARM_MATH_SUCCESS){
        //return 0;
    }
    //return -1;
}
