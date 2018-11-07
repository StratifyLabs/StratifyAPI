#include <errno.h>
#include "dsp/Transform.hpp"
#include "dsp/SignalData.hpp"

using namespace dsp;

FftComplexQ15::FftComplexQ15(u32 n_samples){
	//cfft is initialized using a hack - RFFT init will grab the data needed
	if( arm_dsp_api_q15() && arm_dsp_api_q15()->rfft_init ){
		arm_rfft_instance_q15 rfft_instance;
		if( arm_dsp_api_q15()->rfft_init(&rfft_instance, n_samples, 0, 0) == ARM_MATH_SUCCESS){
			memcpy(instance(), rfft_instance.pCfft, sizeof(*instance()));
		} else {
			set_error_number(EINVAL);
		}
	} else {
		set_error_number(ENOENT);
	}
}

FftComplexQ31::FftComplexQ31(u32 n_samples){
	//cfft is initialized using a hack - RFFT init will grab the data needed
	if( arm_dsp_api_q31() && arm_dsp_api_q31()->rfft_init ){
		arm_rfft_instance_q31 rfft_instance;
		if( arm_dsp_api_q31()->rfft_init(&rfft_instance, n_samples, 0, 0) == ARM_MATH_SUCCESS){
			memcpy(instance(), rfft_instance.pCfft, sizeof(*instance()));
		} else {
			set_error_number(EINVAL);
		}
	} else {
		set_error_number(ENOENT);
	}
}

FftComplexF32::FftComplexF32(u32 n_samples){
	//cfft is initialized using a hack - RFFT init will grab the data needed
	if( arm_dsp_api_f32() && arm_dsp_api_f32()->rfft_fast_init ){
		arm_rfft_fast_instance_f32 rfft_instance;
		if( arm_dsp_api_f32()->rfft_fast_init(&rfft_instance, n_samples) == ARM_MATH_SUCCESS){
			memcpy(instance(), &rfft_instance.Sint, sizeof(*instance()));
		} else {
			set_error_number(EINVAL);
		}
	} else {
		set_error_number(ENOENT);
	}
}

FftRealQ15::FftRealQ15(u32 n_samples){
	if( arm_dsp_api_q15() && arm_dsp_api_q15()->rfft_init ){
		if( arm_dsp_api_q15()->rfft_init(instance(), n_samples, 0, 0) != ARM_MATH_SUCCESS){
			set_error_number(EINVAL);
		}
	} else {
		set_error_number(ENOENT);
	}
}

FftRealQ31::FftRealQ31(u32 n_samples){
	if( arm_dsp_api_q31() && arm_dsp_api_q31()->rfft_init ){
		if( arm_dsp_api_q31()->rfft_init(instance(), n_samples, 0, 0) != ARM_MATH_SUCCESS){
			set_error_number(EINVAL);
		}
	} else {
		set_error_number(ENOENT);
	}
}

FftRealF32::FftRealF32(u32 n_samples){
	if( arm_dsp_api_f32() && arm_dsp_api_f32()->rfft_fast_init ){
		if( arm_dsp_api_f32()->rfft_fast_init(instance(), n_samples) != ARM_MATH_SUCCESS){
			set_error_number(EINVAL);
		}
	} else {
		set_error_number(ENOENT);
	}
}
