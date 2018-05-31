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

BiquadCascadeDf1FilterQ31::BiquadCascadeDf1FilterQ31(const SignalQ31 & coefficients, s8 post_shift){
    m_state.resize( coefficients.count()*4 );
    arm_dsp_api_q31()->biquad_cascade_df1_init(instance(),
                                               coefficients.count(),
                                               (q31_t*)coefficients.vector_data_const(),
                                               m_state.vector_data(),
                                               post_shift);
}

FirDecimateFilterQ31::FirDecimateFilterQ31(const SignalQ31 & coefficients, u8 M, u32 n_samples){
    m_state.resize( coefficients.count() + n_samples - 1);
    arm_dsp_api_q31()->fir_decimate_init(instance(), coefficients.count(), M, (q31_t*)coefficients.vector_data_const(), m_state.vector_data(), n_samples);
    //arm_status (*fir_decimate_init)(arm_fir_decimate_instance_q31 * S, uint16_t numTaps, uint8_t M, q31_t * pCoeffs, q31_t * pState, uint32_t blockSize);

}

