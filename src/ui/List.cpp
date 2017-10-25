//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "sgfx.hpp"
#include "ui/List.hpp"
#include "ui/Button.hpp"

using namespace ui;

List::List(ElementLinked * parent) : ElementLinked(parent) {
	set_scroll_visible();
	set_animation_type(AnimationAttr::PUSH_LEFT);
	m_scroll_animation.set_drawing_motion_total(1000/visible_items());
	m_draw_animation_item = 0;
	m_draw_animation_offset = 0;
	m_select_top_bottom = 0;
}

void List::draw_item_to_scale(const DrawingScaledAttr & attr, sg_size_t x_offset, list_attr_size_t item){
	sg_point_t p1 = attr.point();
	sg_point_t p2 = attr.point();
	sg_dim_t d = attr.dim();
	at(item).draw_to_scale(attr);
	p1.x =- x_offset;
	p2.x += d.width + x_offset;
	attr.bitmap().draw_line(p1, p2);
	p1.y += d.height -1;
	p2.y += d.height - 1;
	attr.bitmap().draw_line(p1, p2);
}

void List::draw_to_scale(const DrawingScaledAttr & attr){
	int i;
	sg_point_t p = attr.point();
	Dim d = attr.dim();
	DrawingScaledAttr item_attr;
	sg_size_t remainder;

	sg_size_t list_item_height;
	sg_point_t top_left;
	int display_items;
	list_attr_size_t index_offset;
	Dim item_d;
	sg_int_t x_offset;

	//list item height
	list_item_height = (d.height() / visible_items());
	remainder = (d.height() % visible_items());
	x_offset = attr.calc_width(25) + p.x;

	index_offset = calc_visible_offset();
	display_items = calc_display_items();
	item_attr.set_dim(d.width() - x_offset, list_item_height );
	item_attr.set_bitmap(attr.bitmap());

	attr.bitmap().clear_rectangle(p,d);

	//draw items on b at x,y within d
	for(i=0; i < display_items; i++){
		//draw each visible item
		item_attr.set_height(list_item_height + remainder);
		top_left.x = x_offset;
		top_left.y = p.y + (item_attr.height())*(i);
		item_attr.set_point(top_left);

		if( m_draw_animation_item < size() ){
			if( m_draw_animation_offset == i ){
				draw_item_to_scale(item_attr, x_offset, m_draw_animation_item);
			}
		} else {

			draw_item_to_scale(item_attr, x_offset, i+index_offset);
			//highlight the selected item
			if( (i+index_offset) == selected() + m_select_top_bottom ){
				attr.bitmap().invert_rectangle( sg_point(p.x, top_left.y+1), sg_dim(d.width(), (list_item_height+remainder)-2));
			}
		}


		if( remainder ){
			remainder--;
		}
	}


	set_redraw_pending();

}

void List::animate_scroll(i8 dir, const DrawingAttr & attr){
	u8 type;
	list_attr_size_t item;

	item = calc_next_visible(dir);

	type = AnimationAttr::NONE;

	if( item < size() ){
		m_scroll_animation.set_drawing_motion_total(scale()/visible_items());
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
			m_scroll_animation.set_step_total(4);
			m_scroll_animation.set_path(AnimationAttr::LINEAR);
		} else {
			m_scroll_animation.set_step_total(6);
			m_scroll_animation.set_path(AnimationAttr::SQUARED_UNDO);
		}
	} else if( (selected() == 0) && (dir < 0) ){
		m_scroll_animation.set_path(AnimationAttr::SQUARED);
		m_scroll_animation.set_step_total(6);
		m_scroll_animation.set_drawing_motion_total((scale()/3)/visible_items());
		type = AnimationAttr::BOUNCE_UP;
		dir = 0;
	} else if( (selected() == (size()-1)) && (dir > 0)){
		m_scroll_animation.set_path(AnimationAttr::SQUARED);
		m_scroll_animation.set_step_total(6);
		m_scroll_animation.set_drawing_motion_total((scale()/3)/visible_items());
		type = AnimationAttr::BOUNCE_DOWN;
		dir = 0;
	}

	if( type != AnimationAttr::NONE ){

		m_scroll_animation.set_type(type);
		m_select_top_bottom = dir; //select the next item as highlighted when drawing the animation
		m_scroll_animation.init(this, 0, attr);

		m_select_top_bottom = 0;
		m_draw_animation_item = item; //this will cause draw to draw the item which will be new on the list
		m_scroll_animation.init(0, this, attr);
		m_scroll_animation.exec();

	}

	m_scroll_timer.restart();
	m_draw_animation_offset = 0;
	m_draw_animation_item = size();
}


void List::handle_down_button_actuation(const Event  & event, const DrawingAttr & attr){
	animate_scroll(1, attr);
	if( selected() < size() - 1){
		inc_selected();
		current().handle_event(Event(Event::LIST_ITEM_SELECTED, &current()), attr);
	}
	draw(attr);
}

void List::handle_up_button_actuation(const Event  & event, const DrawingAttr & attr){
	animate_scroll(-1, attr);
	if( selected() > 0 ){
		dec_selected();
		current().handle_event(Event(Event::LIST_ITEM_SELECTED, &current()), attr); //send the event to the list item
	}
	draw(attr);
}

void List::handle_select_button_actuation(const Event  & event, const DrawingAttr & attr){
	current().handle_event(event, attr);
}


Element * List::handle_event(const Event  & event, const DrawingAttr & attr){
	u32 i;
	switch(event.type()){
	default: break;
	case Event::SETUP:
		m_draw_animation_item = size();
		if( child() && (child() != this) ){
			child()->handle_event(event, attr);
		}
		/* no break */
	case Event::ENTER:
		for(i=0; i < size(); i++){
			at(i).handle_event(event, attr);
		}
		return this;

	case Event::LIST_ACTUATED:
		return current().handle_event(Event(Event::LIST_ITEM_ACTUATED), attr);

	case Event::LIST_UP:
		handle_up_button_actuation(event, attr);
		break;

	case Event::LIST_DOWN:
		handle_down_button_actuation(event, attr);
		break;


	}
	return ElementLinked::handle_event(event, attr);
}
