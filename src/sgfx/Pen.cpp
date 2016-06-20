/*
 * Pen.cpp
 *
 *  Created on: Jun 14, 2016
 *      Author: tgil
 */

#include <sgfx/Pen.hpp>

namespace sgfx {

Pen::Pen() {
	set_color(0xFFFFFFFF);
	set_thickness(1);
	set_fill(false);
	set_mode(SET);
}

Pen::Pen(u32 color, u8 thickness, bool fill, enum mode mode){
	set_color(color);
	set_thickness(thickness);
	set_fill(fill);
	set_mode(mode);
}



} /* namespace sgfx */
