/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include "sgfx.hpp"
#include "sys/Assets.hpp"
#include "ux/draw/Icon.hpp"

using namespace ux::draw;
using namespace sgfx;

Icon::Icon(){
	m_icon_font = nullptr;
	set_align_center();
	set_align_middle();
}

bool Icon::resolve_font(sg_size_t h){
	if( this->icon_font() == nullptr ){
		const IconFontInfo * info =
				sys::Assets::find_icon_font(
					sgfx::Font::Name(m_icon_font_name),
					sgfx::Font::PointSize(h)
					);
		if( info ){
			this->m_icon_font = info->icon_font();
			return true;
		}
		return false;
	}
	return true;
}

void Icon::draw(const DrawingScaledAttributes & attr){
	sg_point_t top_left;
	int w;
	int h;
	Area d = attr.area();
	sg_point_t p = attr.point();
	const IconFont * f;

	if( resolve_font(
				d.height()
				) == false ){
		printf("failed to resolve icon font\n");
		return;
	}

	f = this->icon_font();


	size_t icon_index = f->find(name());
	IconInfo icon_info = f->get_info(icon_index);

	h = icon_info.height();
	w = icon_info.width();

	if( is_align_left() ){
		top_left.x = p.x;
	} else if( is_align_right() ){
		top_left.x = p.x + d.width() - w;
	} else {
		top_left.x = p.x + d.width()/2 - w/2;
	}

	if( is_align_top() ){
		//top
		top_left.y = p.y;
	} else if( is_align_bottom() ){
		//bottom
		top_left.y = p.y + d.height() - h;
	} else {
		//middle
		top_left.y = p.y + d.height()/2 - h/2;
	}

	f->draw(
				icon_index,
				attr.bitmap() << Pen().set_color( m_color ).set_zero_transparent(),
				top_left
				);

}
