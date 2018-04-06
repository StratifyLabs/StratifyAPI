/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONTMEMORY_HPP_
#define SGFX_FONTMEMORY_HPP_


#include "MemoryFont.hpp"

namespace sgfx {

#if !defined __link
[[deprecated("Use MemoryFont")]]
#endif
typedef MemoryFont FontMemory;

}

#endif /* SGFX_FONTMEMORY_HPP_ */
