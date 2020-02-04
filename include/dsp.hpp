/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_DSP_HPP_
#define SAPI_DSP_HPP_

/*! \brief Digital Signal Processing
 * \details The Digital Signal Processing (dsp) namespace
 * includes signal processing algorithms including
 * wrappers and data management tools for using the
 * ARM CMSIS DSP library.
 *
 *
 */
namespace dsp {}

#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"
#include "dsp/Filter.hpp"

using namespace dsp;

#endif // SAPI_DSP_HPP_
