//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "sgfx/Pen.hpp"

namespace sgfx {

Pen::Pen() {
	set_color((sg_color_t)-1);
	set_thickness(1);
	set_fill(false);
}

Pen::Pen(sg_color_t color, u8 thickness, bool fill){
	set_color(color);
	set_thickness(thickness);
	set_fill(fill);
}



} /* namespace sgfx */
