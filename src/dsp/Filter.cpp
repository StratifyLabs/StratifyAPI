#include <errno.h>
#include "dsp/Filter.hpp"
#include "dsp/SignalData.hpp"

using namespace dsp;

FirFilterQ31::FirFilterQ31(const SignalQ31 & coefficients, u32 n_samples){
    m_state.resize( coefficients.count() + n_samples - 1);
    arm_dsp_api_q31()->fir_init(instance(),
                                coefficients.count(),
                                (q31_t*)coefficients.vector_data_const(),
                                m_state.vector_data(),
                                n_samples);
}

BiquadDirectFormOneFilterQ15::BiquadDirectFormOneFilterQ15(const BiquadCoefficientsQ15 & coefficients, s8 post_shift){
    m_state.resize( coefficients.stages()*4 );
    if( arm_dsp_api_q15() && arm_dsp_api_q15()->biquad_cascade_df1_init ){
        arm_dsp_api_q15()->biquad_cascade_df1_init(instance(),
                                                   coefficients.count(),
                                                   (q15_t*)coefficients.vector_data_const(),
                                                   m_state.vector_data(),
                                                   post_shift);
    } else {
        set_error_number(ENOENT);
    }
}

BiquadDirectFormOneFilterQ31::BiquadDirectFormOneFilterQ31(const BiquadCoefficientsQ31 & coefficients, s8 post_shift){
    m_state.resize( coefficients.stages()*4 );
    if( arm_dsp_api_q31() && arm_dsp_api_q31()->biquad_cascade_df1_init ){
        arm_dsp_api_q31()->biquad_cascade_df1_init(instance(),
                                                   coefficients.count(),
                                                   (q31_t*)coefficients.vector_data_const(),
                                                   m_state.vector_data(),
                                                   post_shift);
    } else {
        set_error_number(ENOENT);
    }
}

BiquadDirectFormOneFilterF32::BiquadDirectFormOneFilterF32(const BiquadCoefficientsF32 & coefficients){
    m_state.resize( coefficients.stages()*4 );
    if( arm_dsp_api_f32() && arm_dsp_api_f32()->biquad_cascade_df1_init ){
        arm_dsp_api_f32()->biquad_cascade_df1_init(instance(),
                                                   coefficients.count(),
                                                   (float32_t*)coefficients.vector_data_const(),
                                                   m_state.vector_data());
    } else {
        set_error_number(ENOENT);
    }
}

FirDecimateFilterQ31::FirDecimateFilterQ31(const SignalQ31 & coefficients, u8 M, u32 n_samples){
    m_state.resize( coefficients.count() + n_samples - 1);
    arm_dsp_api_q31()->fir_decimate_init(instance(), coefficients.count(), M, (q31_t*)coefficients.vector_data_const(), m_state.vector_data(), n_samples);
    //arm_status (*fir_decimate_init)(arm_fir_decimate_instance_q31 * S, uint16_t numTaps, uint8_t M, q31_t * pCoeffs, q31_t * pState, uint32_t blockSize);

}

