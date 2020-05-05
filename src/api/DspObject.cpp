/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "api/DspObject.hpp"
#include "sys/requests.h"
#include "sys.hpp"

using namespace api;

#if !defined __link
DspQ7Api DspWorkObject::m_api_q7;
DspQ15Api DspWorkObject::m_api_q15;
DspQ31Api DspWorkObject::m_api_q31;
DspF32Api DspWorkObject::m_api_f32;
DspConversionApi DspWorkObject::m_api_conversion;
#endif

u32 sapi_dsp_object_unused;
