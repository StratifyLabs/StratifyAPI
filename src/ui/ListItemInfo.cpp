//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw.hpp"
#include "ui/ListItemInfo.hpp"

using namespace ui;

ListItemInfo::ListItemInfo(ElementLinked * parent) : ListItem("Label", 0, parent, 0) {
	// TODO Auto-generated constructor stub
	set(0,0);
}

ListItemInfo::ListItemInfo(const char * l, const char * v, ElementLinked * parent) : ListItem(l, 0, parent, 0) {
	// TODO Auto-generated constructor stub
	set(l,v);
}

void ListItemInfo::set(const char * l, const char * v){
	label().assign(l);
	value().assign(v);
}


void ListItemInfo::draw_to_scale(const DrawingScaledAttr & attr){

	int height;
	sg_size_t len;
	sg_dim_t d = attr.d();
	sg_point_t p = attr.p();
	Font * font;



	//draw the label and the icon
	Dim padded;
	padded.set_value(d.width, d.height);

	if( label().font_size() == 0 ){
		height = padded.height();
	} else {
		height = label().font_size();
	}

	font = FontSystem::get_font(height, label().font_bold());
	height = font->get_height();

	if( align_top() ){
		;
	} else if( align_bottom() ){
		p.y = d.height - height;
	} else {
		p.y = p.y + d.height/2 - height/2;
	}

	font->draw_str(label().text(), attr.bitmap(), p);

	//draw the value on the right side
	len = font->calc_len(value().text());
	p.x = p.x + d.width - len - d.width/40;
	font->draw_str(value().text(), attr.bitmap(), p);


}

Element * ListItemInfo::handle_event(const Event  & event, const DrawingAttr & attr){
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
	return Element::handle_event(event, attr);
}

