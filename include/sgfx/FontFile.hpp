/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONTFILE_HPP_
#define SGFX_FONTFILE_HPP_

#include "FileFont.hpp"

namespace sgfx {

#if !defined __link
[[deprecated("Use FileFont")]]
#endif
typedef FileFont FontFile;

}

#endif /* SGFX_FONTFILE_HPP_ */
