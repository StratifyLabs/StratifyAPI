/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include "sgfx/Pen.hpp"

namespace sgfx {

Pen::Pen(){
	m_pen.o_flags = 0;
	set_color((sg_color_t)-1);
	set_thickness(1);
}


} /* namespace sgfx */
