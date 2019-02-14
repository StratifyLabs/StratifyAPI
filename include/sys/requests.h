/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SYS_REQUESTS_H_
#define SYS_REQUESTS_H_

#if !defined __link

#include <mcu/types.h>

/*! \cond */
typedef struct {
	const void * list;
	u16 count;
} sapi_request_asset_t;

typedef struct {
	u32 update_period_milliseconds;
	u32 loop_period_milliseconds;
} sapi_request_hibernate_t;

enum {
	SAPI_REQUEST_FLAG = (1<<30),
	SAPI_REQUEST_ASSET_FONTS /*! Request the kernel to provide fonts */ = (SAPI_REQUEST_FLAG) | 1,
	SAPI_REQUEST_ASSET_VECTOR_ICONS /*! Request the kernel to provide vector icons */ = (SAPI_REQUEST_FLAG) | 2,
	SAPI_REQUEST_ASSET_BITMAP_ICONS /*! Request the kernel to provide bitmap assets */ = (SAPI_REQUEST_FLAG) | 3,
	SAPI_REQUEST_HIBERNATE /*! Request the kernel to enter hibernate */ = (SAPI_REQUEST_FLAG) | 4,
};

enum {
	SAPI_API_REQUEST_MBEDTLS /*! Request theMbed TLS library from the kernel */ = MCU_API_REQUEST_CODE('m','t','l','s'),
	SAPI_API_REQUEST_ARM_DSP_Q7 /*! Request the ARM DSP Q7 API from the kernel (const arm_dsp_api_q7_t *) */ = MCU_API_REQUEST_CODE('d','s','p','0'),
	SAPI_API_REQUEST_ARM_DSP_Q15 /*! Request the ARM DSP Q15 API from the kernel (const arm_dsp_api_q15_t *) */ = MCU_API_REQUEST_CODE('d','s','p','1'),
	SAPI_API_REQUEST_ARM_DSP_Q31 /*! Request the ARM DSP Q31 API from the kernel (const arm_dsp_api_q31_t *) */ = MCU_API_REQUEST_CODE('d','s','p','2'),
	SAPI_API_REQUEST_ARM_DSP_F32 /*! Request the ARM DSP F32 API from the kernel (const arm_dsp_api_f32_t *) */ = MCU_API_REQUEST_CODE('d','s','p','3'),
	SAPI_API_REQUEST_ARM_DSP_CONVERSION /*! Request the ARM DSP Conversion API from the kernel (const arm_dsp_conversion_api_t *) */ = MCU_API_REQUEST_CODE('d','s','p','4'),
	SAPI_API_REQUEST_JSON /*! Request the JSON library (json_api_t) from the kernel */ = MCU_API_REQUEST_CODE('j','s','o','n'),
	SAPI_API_REQUEST_SGFX /*! Request the Stratify Graphics library API from the kernel */ = MCU_API_REQUEST_CODE('s','g','f','x'),
	SAPI_API_REQUEST_SON /*! Request the Stratify Object Notation library API from the kernel */ = MCU_API_REQUEST_CODE('s','o','n','_')
};

#else

#define SAPI_API_REQUEST_MBEDTLS &mbedtls_api
//SAPI_API_REQUEST_ARM_DSP_* not available on Link
#define SAPI_API_REQUEST_SGFX &sg_api
#define SAPI_API_REQUEST_SON &son_api
#define SAPI_API_REQUEST_JSON &jansson_api

#endif
/*! \endcond */


#endif /* SYS_REQUESTS_H_ */
