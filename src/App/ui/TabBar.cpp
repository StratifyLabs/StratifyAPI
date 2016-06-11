//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/TabBar.hpp"
#include "ui/Button.hpp"

using namespace ui;

TabBar::TabBar() {
	m_height = 180;
	m_highlight = 100;
	set_visible();
	m_bounced = false;
}

void TabBar::set_animation_type(u8 v){ m_animation.attr().set_type(v); }
u8 TabBar::animation_type() const { return m_animation.attr().type(); }
void TabBar::set_animation_path(u8 v){ m_animation.attr().set_path(v); }
u8 TabBar::animation_path() const { return m_animation.attr().path(); }

Element * TabBar::handle_event(const Event  & event, const DrawingAttr & attr){
	int i;

	switch(event.type()){
	case Event::SETUP:
		for(i=0; i < size(); i++){
			at(i)->element()->handle_event(event, attr);
		}
		break;
	case Event::ENTER:
		m_animation.attr().set_drawing_start(0,0);
		m_animation.attr().set_drawing_dim(1000,1000);
		m_animation.attr().set_step_total(8);
		m_animation.attr().set_drawing_motion_total(1000);

		//animate the tab bar
		m_animation.init(0, this);
		m_animation.exec();

		//subsequent animations will only deal with the screen area
		m_animation.attr().set_drawing_start(0, h());
		m_animation.attr().set_drawing_dim(1000, 1000-h());
		at(selected())->element()->handle_event(event, attr);
		return this;

	case Event::BUTTON_ACTUATION:

		if(1){
			switch(event.button()->event_id()){

			case Event::RIGHT_BUTTON:
				if( at(selected())->element()->handle_event(event, attr) == 0 ){
					scroll(1, false, attr);
				}
				break;

			case Event::LEFT_BUTTON:
				if( at(selected())->element()->handle_event(event, attr) == 0 ){
					scroll(-1, false, attr);
				}
				break;
			default:
				break;
			}
		}
		return this;

	case Event::UPDATE:
		/*
			if( Button::right_held() > Button::long_press()*3 ){
				scroll(1, true, attr);
				return this;
			} else if( Button::left_held() > Button::long_press()*3 ){
				scroll(-1, true, attr);
				return this;
			}
		 */

		break;
	default:
		break;
	}

	if( at(selected())->element()->handle_event(event, attr) > 0 ){
		return this;
	}

	return 0;
}

void TabBar::scroll(int dir, bool repeat, const DrawingAttr & attr){
	Tab * t = at(selected());
	Element * current;
	current = t->element();
	u8 next_selected;

	next_selected = selected();

	if( dir > 0 ){
		if( selected() < size() - 1 ){
			next_selected = selected() + 1;
			t = at(next_selected);
			if( current != t->element() ){
				t->element()->handle_event(Event(Event::ENTER), attr);
				set_animate_push(AnimationAttr::PUSH_LEFT, repeat);
			}
		} else {
			if( repeat && m_bounced ){
				return;
			}
			set_animate_bounce(AnimationAttr::BOUNCE_RIGHT);
		}
	} else {
		if( selected() ){
			next_selected = selected() - 1;
			t = at(next_selected);
			if( current != t->element() ){
				t->element()->handle_event(Event(Event::ENTER), attr);
				set_animate_push(AnimationAttr::PUSH_RIGHT, repeat);
			}
		} else {
			if( repeat && m_bounced ){
				return;
			}
			set_animate_bounce(AnimationAttr::BOUNCE_LEFT);
		}
	}

	m_animation.init(0, t->element());
	m_animation.exec();
	set_selected(next_selected);

	if( (selected() > 0) && (selected() < (size()-1)) ){
		m_bounced = false;
	}

}

void TabBar::set_animate_bounce(int type){
	m_animation.attr().set_path(AnimationAttr::SQUARED);
	m_animation.attr().set_type(type);
	m_animation.attr().set_drawing_motion_total(100);
	m_animation.attr().set_step_total(4);
	m_bounced = true;
}

void TabBar::set_animate_push(int type, bool repeat){
	u8 step_mult = 3;
	m_animation.attr().set_path(AnimationAttr::SQUARED);
	if( repeat ){
		m_animation.attr().set_path(AnimationAttr::LINEAR);
		step_mult = 1;
	}
	m_animation.attr().set_type(type);
	m_animation.attr().set_drawing_motion_total(1000);
	m_animation.attr().set_step_total(3*step_mult);
}


void TabBar::draw(const DrawingAttr & attr){
	Tab * t;
	DrawingAttr element_attr = attr;

	t = at(selected());

	if( visible() ){
		element_attr.set_point( drawing_point(attr.x(), attr.y() + h()) );
		element_attr.set_dim( drawing_dim(1000, attr.h() - attr.calc_h( h() ) ));
		t->element()->draw(element_attr);
		draw_tab_bar(attr + drawing_dim(1000, h()), selected());
	} else {
		t->element()->draw(attr);
	}

}




void TabBar::draw_tab_bar(const DrawingAttr & attr, int selected){
	list_attr_size_t tabs;
	Tab * t;
	//DrawingAttr tab_attr;
	list_attr_size_t offset;
	list_attr_size_t offset_tab;
	drawing_size_t tab_width = 1000 / visible_items();

	//draw the screen under the tab bar

	clear(attr);

	offset = calc_visible_offset();

	//draw the tabs
	for(tabs = 0; tabs < visible_items(); tabs++){
		offset_tab = tabs+offset;
		t = at(offset_tab);
		t->draw(attr + drawing_point(tab_width * tabs, 0) + drawing_dim(tab_width, 800));
	}



	Drawing::set(attr + drawing_point(tab_width*(selected - offset) + tab_width/4, 1000 - highlight()) + drawing_dim(tab_width/2, highlight()));
}


