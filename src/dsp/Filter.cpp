#include <errno.h>
#include "dsp/Filter.hpp"
#include "dsp/SignalData.hpp"

using namespace dsp;

FirFilterQ15::FirFilterQ15(const SignalQ15 & coefficients, u32 n_samples){
	m_state.resize(coefficients.count() + n_samples - 1);
	api_q15()->fir_init(
				instance(),
				coefficients.count(),
				(q15_t*)coefficients.to_const_void(),
				m_state.data(),
				n_samples);
}
FirFilterQ31::FirFilterQ31(const SignalQ31 & coefficients, u32 n_samples){
	m_state.resize( (coefficients.count() + n_samples - 1) );
	api_q31()->fir_init(
				instance(),
				coefficients.count(),
				(q31_t*)coefficients.to_const_void(),
				m_state.data(),
				n_samples);
}

FirFilterF32::FirFilterF32(const SignalF32 & coefficients, u32 n_samples){
	m_state.resize( (coefficients.count() + n_samples - 1) );
	api_f32()->fir_init(
				instance(),
				coefficients.count(),
				(float32_t*)coefficients.to_const_void(),
				m_state.data(),
				n_samples);
}

BiquadFilterQ15::BiquadFilterQ15(const BiquadCoefficientsQ15 & coefficients, s8 post_shift){
	m_state.resize( (coefficients.stages()*4) );
	if( api_q15().is_valid() && api_q15()->biquad_cascade_df1_init ){
		api_q15()->biquad_cascade_df1_init(
					instance(),
					coefficients.count(),
					(q15_t*)coefficients.to_const_void(),
					m_state.data(),
					post_shift);
	} else {
		set_error_number(ENOENT);
	}
}

BiquadFilterQ31::BiquadFilterQ31(const BiquadCoefficientsQ31 & coefficients, s8 post_shift){
	m_state.resize( (coefficients.stages()*4) );
	if( api_q31().is_valid() && api_q31()->biquad_cascade_df1_init ){
		api_q31()->biquad_cascade_df1_init(
					instance(),
					coefficients.count(),
					(q31_t*)coefficients.to_const_void(),
					m_state.data(),
					post_shift);
	} else {
		set_error_number(ENOENT);
	}
}

BiquadFilterF32::BiquadFilterF32(const BiquadCoefficientsF32 & coefficients){
	m_state.resize( (coefficients.stages()*4) );
	if( api_f32().is_valid() && api_f32()->biquad_cascade_df1_init ){
		api_f32()->biquad_cascade_df1_init(
					instance(),
					coefficients.count(),
					(float32_t*)coefficients.to_const_void(),
					m_state.data()
					);
	} else {
		set_error_number(ENOENT);
	}
}

FirDecimateFilterQ31::FirDecimateFilterQ31(const SignalQ31 & coefficients, u8 M, u32 n_samples){
	m_state.resize( (coefficients.count() + n_samples - 1) );
	api_q31()->fir_decimate_init(
				instance(),
				coefficients.count(),
				M,
				(q31_t*)coefficients.to_const_void(),
				m_state.data(),
				n_samples
				);
	//arm_status (*fir_decimate_init)(arm_fir_decimate_instance_q31 * S, uint16_t numTaps, uint8_t M, q31_t * pCoeffs, q31_t * pState, uint32_t blockSize);

}

