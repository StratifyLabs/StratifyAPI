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
	sg_point_t p = attr.p();
	Dim d = attr.d();
	int bar_size;
	sg_dim_t bar;
	bar_size = d.height() / total;
	attr.b().draw_rectangle(p, d);
	p.y = p.y + selected*bar_size;
	bar.width = d.width();
	bar.height = bar_size;
	attr.b().clear_rectangle(p,bar);
}

Element * Element::handle_event(const Event & event, const DrawingAttr & attr){
	return this;
}

void Element::set_animation_type(u8 v){}
u8 Element::animation_type() const{ return AnimationAttr::PUSH_LEFT; }
void Element::set_animation_path(u8 path){}
void Element::set_animation(u8 type, u8 path){ set_animation_type(type); set_animation_path(path); }
u8 Element::animation_patheight() const { return AnimationAttr::SQUARED; }
