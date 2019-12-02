
#include "sys/Assets.hpp"
#include "draw/TextAttr.hpp"
using namespace sgfx;
using namespace draw;


const Font * TextAttr::resolve_font(sg_size_t h) const{
	const FontInfo * info = sys::Assets::find_font(
				Font::PointSize(h),
				Font::Style(m_font_style)
				);
	if( info ){ return info->font(); }
	return 0;
}
