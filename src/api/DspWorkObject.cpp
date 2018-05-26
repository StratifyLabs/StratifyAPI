#include "api/DspObject.hpp"
#include "sys/requests.h"
#include "sys.hpp"

using namespace api;

const arm_dsp_api_q7_t * DspWorkObject::m_arm_dsp_api_q7;
const arm_dsp_api_q15_t * DspWorkObject::m_arm_dsp_api_q15;
const arm_dsp_api_q31_t * DspWorkObject::m_arm_dsp_api_q31;
const arm_dsp_api_f32_t * DspWorkObject::m_arm_dsp_api_f32;
const arm_dsp_conversion_api_t * DspWorkObject::m_arm_dsp_conversion_api;


int DspWorkObject::request_arm_dsp_api(){
    sapi_request_arm_dsp_api_t api_request;
    int ret = Sys::request(SAPI_REQUEST_ARM_DSP_API, &api_request);
    if( ret >= 0 ){
        if( api_request.api_q7 ){ m_arm_dsp_api_q7 = api_request.api_q7; }
        if( api_request.api_q15 ){ m_arm_dsp_api_q15 = api_request.api_q15; }
        if( api_request.api_q31 ){ m_arm_dsp_api_q31 = api_request.api_q31; }
        if( api_request.api_f32 ){ m_arm_dsp_api_f32 = api_request.api_f32; }
        if( api_request.conversion_api ){ m_arm_dsp_conversion_api = api_request.conversion_api; }
    }
    return ret;
}
