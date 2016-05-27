//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw.hpp"
#include "ui/ListItemInfo.hpp"

using namespace ui;

ListItemInfo::ListItemInfo(ElementLinked * parent) : ElementLinked(parent) {
	// TODO Auto-generated constructor stub
	set(0,0);
}

ListItemInfo::ListItemInfo(const char * l, const char * v, ElementLinked * parent) : ElementLinked(parent) {
	// TODO Auto-generated constructor stub
	set(l,v);
}

void ListItemInfo::set(const char * l, const char * v){
	set_label(l);
	set_value(v);
}

void ListItemInfo::set_label(const char * v){
	m_label.assign(v);
}

void ListItemInfo::set_value(const char * v){
	m_value.assign(v);
}


void ListItemInfo::draw_to_scale(const DrawingScaledAttr & attr){

	int height;
	sg_size_t len;
	sg_dim_t d = attr.d();
	sg_point_t p = attr.p();
	Bitmap * b = attr.b();

	char buffer[PATH_MAX];

	/*
	//draw the label and the icon
	Dim padded;
	sprintf(buffer, "%s %s", label(), value());
	padded.set_dim(d.w, d.h);

	if( font_size() == 0 ){
		height = App::select_system_font(padded, buffer, font_bold());
	} else {
		height = App::select_system_font(font_size(), font_bold());
	}

	if( align_top() ){
		;
	} else if( align_bottom() ){
		p.y = d.h - height;
	} else {
		p.y = p.y + d.h/2 - height/2;
	}

	App::font()->set_str(label(), attr.b(), p);

	//draw the value on the right side
	len = App::font()->len(value());
	p.x = p.x + d.w - len - d.w/40;
	App::font()->set_str(value(), b, p);
	*/

}

Element * ListItemInfo::event_handler(int event, const DrawingAttr & attr){
	/*
	switch(event){
	case RIGHT_HOLD:
	case CENTER_PRESS:
		return parent();
	case RIGHT_PRESS:
	case LEFT_PRESS:
		App::bar().set_center_visible(false);
		break;
	}
	*/
	return Element::event_handler(event, attr);
}

