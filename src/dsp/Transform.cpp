#include "dsp/Transform.hpp"
#include "dsp/SignalData.hpp"

using namespace dsp;

int CfftQ15::init(const SignalQ15 & data){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    arm_rfft_instance_q15 rfft_instance;
    if( arm_dsp_api_q15()->rfft_init(&rfft_instance, data.count(), 0, 0) == ARM_MATH_SUCCESS){
        m_instance = rfft_instance.pCfft;
        return 0;
    }
    return -1;
}

int CfftQ31::init(const SignalQ31 & data){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    arm_rfft_instance_q31 rfft_instance;
    if( arm_dsp_api_q31()->rfft_init(&rfft_instance, data.count(), 0, 0) == ARM_MATH_SUCCESS){
        m_instance = rfft_instance.pCfft;
        return 0;
    }
    return -1;
}

int CfftF32::init(const SignalF32 & data){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    arm_rfft_fast_instance_f32 rfft_instance;
    if( arm_dsp_api_f32()->rfft_fast_init(&rfft_instance, data.count()) == ARM_MATH_SUCCESS){
        m_instance = rfft_instance.Sint;
        return 0;
    }
    return -1;
}

int RfftQ15::init(const SignalQ15 & data){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    if( arm_dsp_api_q15()->rfft_init(&m_instance, data.count(), 0, 0) == ARM_MATH_SUCCESS){
        return 0;
    }
    return -1;
}

int RfftQ31::init(const SignalQ31 & data){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    if( arm_dsp_api_q31()->rfft_init(&m_instance, data.count(), 0, 0) == ARM_MATH_SUCCESS){
        return 0;
    }
    return -1;
}

int RfftF32::init(const SignalF32 & data){
    //cfft is initialized using a hack - RFFT init will grab the data needed
    if( arm_dsp_api_f32()->rfft_fast_init(&m_instance, data.count()) == ARM_MATH_SUCCESS){
        return 0;
    }
    return -1;
}
