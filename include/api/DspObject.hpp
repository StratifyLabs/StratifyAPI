#ifndef API_DSP_OBJECT_HPP
#define API_DSP_OBJECT_HPP

#if !defined __link

#include <arm_dsp_api.h>
#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Event Informatin Object
 *
 */
class DspInfoObject : public virtual InfoObject {

};

/*! \brief Event Object Class
 */
class DspWorkObject : public virtual WorkObject {
public:

    static const arm_dsp_api_q7_t * arm_dsp_api_q7(){ return m_arm_dsp_api_q7; }
    static const arm_dsp_api_q15_t * arm_dsp_api_q15(){ return m_arm_dsp_api_q15; }
    static const arm_dsp_api_q31_t * arm_dsp_api_q31(){ return m_arm_dsp_api_q31; }
    static const arm_dsp_api_f32_t * arm_dsp_api_f32(){ return m_arm_dsp_api_f32; }
    static const arm_dsp_conversion_api_t * arm_dsp_conversion_api(){ return m_arm_dsp_conversion_api; }

    static void set_arm_dsp_api_q7(const arm_dsp_api_q7_t * api){ m_arm_dsp_api_q7 = api; }
    static void set_arm_dsp_api_q15(const arm_dsp_api_q15_t * api){ m_arm_dsp_api_q15 = api; }
    static void set_arm_dsp_api_q31(const arm_dsp_api_q31_t * api){ m_arm_dsp_api_q31 = api; }
    static void set_arm_dsp_api_f32(const arm_dsp_api_f32_t * api){ m_arm_dsp_api_f32 = api; }
    static void set_arm_dsp_conversion_api(const arm_dsp_conversion_api_t * api){ m_arm_dsp_conversion_api = api; }

    static int request_arm_dsp_api();

protected:
    static const arm_dsp_api_q7_t * m_arm_dsp_api_q7;
    static const arm_dsp_api_q15_t * m_arm_dsp_api_q15;
    static const arm_dsp_api_q31_t * m_arm_dsp_api_q31;
    static const arm_dsp_api_f32_t * m_arm_dsp_api_f32;
    static const arm_dsp_conversion_api_t * m_arm_dsp_conversion_api;

};


}

#endif

#endif // API_DSP_OBJECT_HPP
