#ifndef SAPI_API_DSP_OBJECT_HPP_
#define SAPI_API_DSP_OBJECT_HPP_

#if !defined __link

#include <arm_dsp_api.h>
#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../sys/requests.h"

namespace api {

/*! \brief DSP Information Object
 *
 * \details
 * \sa dsp namespace
 *
 */
class DspInfoObject : public virtual InfoObject {

};

typedef api::Api<arm_dsp_api_q7_t, SAPI_API_REQUEST_ARM_DSP_Q7> DspQ7Api;
typedef api::Api<arm_dsp_api_q15_t, SAPI_API_REQUEST_ARM_DSP_Q15> DspQ15Api;
typedef api::Api<arm_dsp_api_q31_t, SAPI_API_REQUEST_ARM_DSP_Q31> DspQ31Api;
typedef api::Api<arm_dsp_api_f32_t, SAPI_API_REQUEST_ARM_DSP_F32> DspF32Api;
typedef api::Api<arm_dsp_conversion_api_t, SAPI_API_REQUEST_ARM_DSP_CONVERSION> DspConversionApi;


/*! \brief DSP Work Object
 *
 * \details
 * \sa dsp namespace
 *
 */
class DspWorkObject : public virtual WorkObject {
public:

	static DspQ7Api & api_q7(){ return m_api_q7; }
	static DspQ15Api & api_q15(){ return m_api_q15; }
	static DspQ31Api & api_q31(){ return m_api_q31; }
	static DspF32Api & api_f32(){ return m_api_f32; }
	static DspConversionApi & api_conversion(){ return m_api_conversion; }

protected:

	static DspQ7Api m_api_q7;
	static DspQ15Api m_api_q15;
	static DspQ31Api m_api_q31;
	static DspF32Api m_api_f32;
	static DspConversionApi m_api_conversion;

};


}

#endif

#endif // SAPI_API_DSP_OBJECT_HPP_
