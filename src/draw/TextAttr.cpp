
#include "sgfx/FontSystem.hpp"
#include "draw/TextAttr.hpp"
using namespace sgfx;
using namespace draw;


const Font * TextAttr::resolve_font(sg_size_t h) const{
	const Font * ret;
	sg_size_t font_height;
	if( font() == 0 ){
		if( font_size() == 0 ){
			font_height = h;
		} else {
			font_height = font_size();
		}
		ret = FontSystem::get_font(font_height, font_bold());
	} else {
		ret = font();
	}
	return ret;
}
