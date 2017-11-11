//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "ui/Element.hpp"
#include "draw.hpp"

using namespace ui;

Element::Element(){}

void Element::draw_scroll(const DrawingScaledAttr & attr,
		int selected,
		int total,
		int visible){
	sg_point_t p = attr.point();
	Dim d = attr.dim();
	int bar_size;
	sg_dim_t bar;
	bar_size = d.height() / total;
	attr.bitmap().draw_rectangle(p, d);
	p.y = p.y + selected*bar_size;
	bar.width = d.width();
	bar.height = bar_size;
	attr.bitmap().clear_rectangle(p,bar);
}

Element * Element::handle_event(const Event & event, const DrawingAttr & attr){
	return this;
}

void Element::set_animation_type(u8 v){}
void Element::set_animation_frame_delay(u16 delay){}
u8 Element::animation_type() const{ return AnimationAttr::PUSH_LEFT; }
void Element::set_animation_path(u8 path){}

void Element::set_animation(u8 type, u8 path, u16 delay){
	set_animation_type(type);
	set_animation_path(path);
	if( delay != (u16)-1 ){
		set_animation_frame_delay(delay);
	}
}

u8 Element::animation_path() const { return AnimationAttr::SQUARED; }
u16 Element::animation_frame_delay() const { return 16; }
