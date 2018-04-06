/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SGFX_FONTSVGMEMORY_HPP_
#define SGFX_FONTSVGMEMORY_HPP_

#include "FontSvg.hpp"

namespace sgfx {

#if !defined __link
[[deprecated("Use SvgMemoryFont")]]
#endif
typedef SvgMemoryFont FontSvgMemory;

}

#endif /* SGFX_FONTSVGMEMORY_HPP_ */
