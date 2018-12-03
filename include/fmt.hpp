/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef FMT_HPP_
#define FMT_HPP_

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

#endif /* FMT_HPP_ */
