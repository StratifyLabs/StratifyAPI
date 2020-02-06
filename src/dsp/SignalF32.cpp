/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

#define IS_FLOAT 1
#define native_type float32_t
#define arm_dsp_api_function() api_f32()
#define SignalDataType SignalDataF32
#define SignalType SignalF32
#define SignalComplexType SignalComplexF32
#define FftRealType FftRealF32
#define FftComplexType FftComplexF32
#define BiquadFilterType BiquadFilterF32
#define FirFilterType FirFilterF32
#define big_type float32_t

#include "SignalDataGeneric.h"

