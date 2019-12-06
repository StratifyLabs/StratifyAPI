//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "sgfx/Pen.hpp"

namespace sgfx {

Pen::Pen(){
	set_color((sg_color_t)-1);
	set_thickness(1);
	set_fill(false);
}


} /* namespace sgfx */
