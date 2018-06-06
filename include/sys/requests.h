/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SYS_REQUESTS_H_
#define SYS_REQUESTS_H_

#if !defined __link

#include <mcu/types.h>
#include <arm_dsp_api.h>

typedef struct {
	const void * list;
	u16 count;
} sapi_request_asset_t;

typedef struct {
	u32 update_period;
	u32 loop_period;
} sapi_request_hibernate_t;

typedef struct {
    const arm_dsp_api_q7_t * api_q7;
    const arm_dsp_api_q15_t * api_q15;
    const arm_dsp_api_q31_t * api_q31;
    const arm_dsp_api_f32_t * api_f32;
    const arm_dsp_conversion_api_t * conversion_api;
} sapi_request_arm_dsp_api_t;

enum {
	SAPI_REQUEST_FLAG = (1<<30),
    SAPI_REQUEST_ASSET_FONTS /*! Request the kernel to provide fonts */ = (SAPI_REQUEST_FLAG) | 1,
    SAPI_REQUEST_ASSET_VECTOR_ICONS /*! Request the kernel to provide vector icons */ = (SAPI_REQUEST_FLAG) | 2,
    SAPI_REQUEST_ASSET_BITMAP_ICONS /*! Request the kernel to provide bitmap assets */ = (SAPI_REQUEST_FLAG) | 3,
    SAPI_REQUEST_HIBERNATE /*! Request the kernel to enter hibernate */ = (SAPI_REQUEST_FLAG) | 4,
    SAPI_REQUEST_ARM_DSP_API /*! Request the ARM DSP API from the kernel */ = (SAPI_REQUEST_FLAG) | 5
};

#endif


#endif /* SYS_REQUESTS_H_ */
