/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_FMT_HPP_
#define SAPI_FMT_HPP_

/*! \brief File Formats
 *
 */
namespace fmt {}

#include "fmt/Bmp.hpp"
#include "fmt/Wav.hpp"
#include "fmt/Son.hpp"
#include "fmt/Svic.hpp"

#if !defined __link
#include "fmt/Xml.hpp"
#endif

using namespace fmt;

#endif /* SAPI_FMT_HPP_ */
