//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "sgfx.hpp"
#include "ui/List.hpp"
#include "ui/Button.hpp"

using namespace ui;

List::List(ElementLinked * parent) : ElementLinked(parent) {
	set_scroll_visible();
	set_animation_type(AnimationAttr::PUSH_LEFT);
	m_scroll_animation.attr().set_drawing_motion_total(1000/visible_items());
	m_draw_animation_item = 0;
	m_draw_animation_offset = 0;
	m_select_top_bottom = 0;
	m_attr_h = 1000;
}

void List::draw_item_to_scale(const DrawingScaledAttr & attr, sg_size_t x_offset, list_attr_size_t item){
	sg_point_t p = attr.p();
	sg_dim_t d = attr.d();
	at(item)->draw_to_scale(attr);
	attr.bitmap().set_hline(p.x - x_offset, p.x + d.w + x_offset, p.y);
	attr.bitmap().set_hline(p.x - x_offset, p.x + d.w + x_offset, p.y + d.h -1);
}


void List::draw(const DrawingAttr & attr){
	m_scroll_animation.attr().set_drawing_start(attr.p());
	m_scroll_animation.attr().set_drawing_dim(attr.d());
	//scroll_animation.attr().set_drawing_motion_total(attr.h()/visible_items());
	m_attr_h = attr.h();

	ElementLinked::draw(attr);
}

void List::draw_to_scale(const DrawingScaledAttr & attr){
	int i;
	sg_point_t p = attr.p();
	Dim d = attr.d();
	DrawingScaledAttr item_attr;
	sg_size_t remainder;

	sg_size_t list_item_height;
	sg_point_t top_left;
	int display_items;
	list_attr_size_t index_offset;
	Dim item_d;
	sg_int_t x_offset;

	//list item height
	list_item_height = (d.h() / visible_items());
	remainder = (d.h() % visible_items());
	x_offset = attr.calc_w(25) + p.x;

	index_offset = calc_visible_offset();
	display_items = calc_display_items();
	item_attr.set_dim(d.w() - x_offset, list_item_height );
	item_attr.set_bitmap(attr.bitmap());

	attr.bitmap().clear(p,d);

	//draw items on b at x,y within d
	for(i=0; i < display_items; i++){
		//draw each visible item
		item_attr.set_height(list_item_height + remainder);
		top_left.x = x_offset;
		top_left.y = p.y + (item_attr.h())*(i);
		item_attr.set_point(top_left);

		if( m_draw_animation_item < size() ){
			if( m_draw_animation_offset == i ){
				draw_item_to_scale(item_attr, x_offset, m_draw_animation_item);
			}
		} else {

			draw_item_to_scale(item_attr, x_offset, i+index_offset);
			//highlight the selected item
			if( (i+index_offset) == selected() + m_select_top_bottom ){
				attr.bitmap().invert( sg_point(p.x, top_left.y+1), sg_dim(d.w(), (list_item_height+remainder)-2));
			}
		}


		if( remainder ){
			remainder--;
		}
	}

	if( !dark() ){
		attr.bitmap().invert(p, d);
	}

}

void List::animate_scroll(i8 dir){
	u8 type;
	list_attr_size_t item;

	item = calc_next_visible(dir);

	type = AnimationAttr::NONE;


	if( item < size() ){
		m_scroll_animation.attr().set_drawing_motion_total(m_attr_h/visible_items());
		if( dir < 0 ){
			m_draw_animation_offset = visible_items() - 1;
		} else {
			m_draw_animation_offset = 0;
		}

		//enter the tab bar
		if( dir < 0 ){
			type = AnimationAttr::PUSH_DOWN;
		} else {
			type = AnimationAttr::PUSH_UP;
		}

		if( m_scroll_timer.msec() < 250 ){
			m_scroll_animation.attr().set_step_total(3);
			m_scroll_animation.attr().set_path(AnimationAttr::LINEAR);
		} else {
			m_scroll_animation.attr().set_step_total(6);
			m_scroll_animation.attr().set_path(AnimationAttr::SQUARED);
		}
	} else if( (selected() == 0) && (dir < 0) ){
		m_scroll_animation.attr().set_path(AnimationAttr::SQUARED);
		m_scroll_animation.attr().set_step_total(3);
		m_scroll_animation.attr().set_drawing_motion_total((m_attr_h/3)/visible_items());
		type = AnimationAttr::BOUNCE_UP;
		dir = 0;
	} else if( (selected() == (size()-1)) && (dir > 0)){
		m_scroll_animation.attr().set_path(AnimationAttr::SQUARED);
		m_scroll_animation.attr().set_step_total(3);
		m_scroll_animation.attr().set_drawing_motion_total((m_attr_h/3)/visible_items());
		type = AnimationAttr::BOUNCE_DOWN;
		dir = 0;
	}

	if( type != AnimationAttr::NONE ){

		m_scroll_animation.attr().set_type(type);
		m_select_top_bottom = dir;
		m_scroll_animation.init(this, 0);

		m_select_top_bottom = 0;
		m_draw_animation_item = item;
		m_scroll_animation.init(0, this);
		m_scroll_animation.exec();

	}


	m_scroll_timer.start();
	m_draw_animation_offset = 0;
	m_draw_animation_item = size();
}



Element * List::handle_event(const Event  & event, const DrawingAttr & attr){
	size_t i;
	switch(event.type()){
	case Event::SETUP:
		Element::handle_event(event, attr); //sets dim of this item
		m_draw_animation_item = size();
		//setup all the items in the list
		for(i=0; i < size(); i++){
			at(i)->handle_event(event, attr);
		}
		return this;


	case Event::BUTTON_ACTUATION:

		if(1){
			switch(event.button()->event_id()){

			case Event::SELECT_BUTTON:
				//invoke menu event handler on a transition
				return current()->handle_event(event, attr);

			case Event::DOWN_BUTTON:
				animate_scroll(1);
				if( selected() < size() - 1){
					inc_selected();
				}
				current()->handle_event(event, attr);

				return this;
			case Event::UP_BUTTON:
				animate_scroll(-1);
				if( selected() > 0 ){
					dec_selected();
				}
				current()->handle_event(event, attr);
				return this;
			default:
				break;
			}
		}
		break;

	case Event::ENTER:
		//App::set_update_period(0);
		//App::bar().set_left( IconAttr::CHEVRON, 3, SG_TRIG_POINTS/4 );
		//App::bar().set_right( IconAttr::CHEVRON, 3, SG_TRIG_POINTS*3/4 );
		//App::bar().set_center( IconAttr::CHEVRON ); //set this based on the selected item
		current()->handle_event(event, attr);
		return this;
	case Event::UPDATE:
		//execute the animation
		return this;

	default:
		break;
	}
	return ElementLinked::handle_event(event, attr);
}
