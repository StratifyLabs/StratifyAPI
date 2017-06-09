//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "hal/Display.hpp"

namespace hal {

void DisplayPalette::set_color(size_t v, u8 r, u8 g, u8 b){
	u8 * ptr = color(v);
	if( ptr == 0 ){
		return;
	}

	switch(pixel_format()){
	default:
	case PIXEL_FORMAT_1BPP: break;
	case PIXEL_FORMAT_RGB565:
		break;
	case PIXEL_FORMAT_RGB666:
		break;
	case PIXEL_FORMAT_RGB888:
		ptr[0] = r;
		ptr[1] = g;
		ptr[2] = b;
		break;
	}

}

Display::Display(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h) : sgfx::Bitmap(mem,w,h){}

} /* namespace hal */
