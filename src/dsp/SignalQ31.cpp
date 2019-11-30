#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

#define IS_FLOAT 0
#define unsigned_native_type u32
#define native_type q31_t
#define LOCAL_INT_MAX INT32_MAX
#define arm_dsp_api_function() api_q31()
#define SignalDataType SignalDataQ31
#define SignalType SignalQ31
#define SignalComplexType SignalComplexQ31
#define FftRealType FftRealQ31
#define FftComplexType FftComplexQ31
#define BiquadFilterType BiquadFilterQ31
#define FirFilterType FirFilterQ31
#define big_type q63_t

#include "SignalDataGeneric.h"

SignalQ31 SignalQ31::filter(const FirDecimateFilterQ31 & filter){
	SignalQ31 ret = SignalQ31(arg::Count(count()));
	api_q31()->fir_decimate_fast(
				(arm_fir_decimate_instance_q31*)filter.instance(),
				(q31_t*)to_const_void(),
				ret.to<q31_t>(),
				count()
				);

	return ret;
}

void SignalQ31::filter(SignalQ31 & output, const FirDecimateFilterQ31 & filter){
	api_q31()->fir_decimate_fast(
				(arm_fir_decimate_instance_q31*)filter.instance(),
				(q31_t*)to_const_void(),
				output.to<q31_t>(),
				count()
				);
}

