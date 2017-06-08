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
	set_mode(NONE);
}

Pen::Pen(sg_color_t color, u8 thickness, bool fill){
	set_color(color);
	set_thickness(thickness);
	set_fill(fill);
}



} /* namespace sgfx */
