/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw/Animation.hpp"
using namespace draw;

AnimationAttr::AnimationAttr(){
	attr().type = PUSH_LEFT;
	attr().path.type = SQUARED;
	attr().path.step_total = 10;
	attr().path.motion_total = 100;
	m_drawing_attr.dim.w = 1000;
	m_drawing_attr.dim.h = 1000;
	m_drawing_attr.start.x = 0;
	m_drawing_attr.start.y = 0;
	m_drawing_attr.motion_total = 1000;
}

Animation::Animation() {
	// TODO Auto-generated constructor stub
	m_animate_bar = false;
}

void Animation::init(Drawing * current,
		Drawing * target){

	DrawingAttr drawing_attr;
	//App::display().wait_busy();

	if( current != 0 ){
		//draw current on display screen
		//App::set_draw_display();
		//drawing_attr.set(&(App::display_screen()), attr().drawing_start(), attr().drawing_dim());
		//App::draw(current, drawing_attr);
	}

	//draw target on animation screen
	if( target != 0 ){
		//App::set_draw_scratch();
		//drawing_attr.set(&(App::display_screen()), attr().drawing_start(), attr().drawing_dim());
		//target->draw(drawing_attr);
	}

	//scale the dimensions
	if( animate_bar() ){
		//drawing_attr.set_bitmap(&App::display());
		//App::set_draw_scratch();
		//App::bar().draw( &(App::display_bar()), 0, 0, 1000, 1000 );
	} else {
		//App::set_draw_display();
		//App::bar().draw( &(App::display_bar()), 0, 0, 1000, 1000 );
	}

	if( current != 0 ){
		//App::display().set_pending();
	}

	//App::set_draw_display();

	update_motion_total();

}

void Animation::update_motion_total(void){
	DrawingAttr drawing_attr;

	//drawing_attr.set(&(App::display_screen()), attr().drawing_start(), attr().drawing_dim());

	//if( animate_bar() ){
	//	drawing_attr.set_bitmap(&App::display());
	//}

	sg_dim_t d;
	sg_point_t p;

	d = Drawing::dim_on_bitmap( drawing_attr );
	p = Drawing::point_on_bitmap( drawing_attr );

	//convert motion total
	switch(attr().type()){
	case AnimationAttr::PUSH_DOWN:
	case AnimationAttr::PUSH_UP:
	case AnimationAttr::SLIDE_DOWN:
	case AnimationAttr::UNDO_SLIDE_DOWN:
	case AnimationAttr::SLIDE_UP:
	case AnimationAttr::UNDO_SLIDE_UP:
	case AnimationAttr::BOUNCE_UP:
	case AnimationAttr::BOUNCE_DOWN:
		//convert motion total as a height
		attr().set_motion_total( attr().drawing_motion_total() * d.h / 1000 );
		break;
	case AnimationAttr::PUSH_LEFT:
	case AnimationAttr::PUSH_RIGHT:
	case AnimationAttr::SLIDE_LEFT:
	case AnimationAttr::UNDO_SLIDE_LEFT:
	case AnimationAttr::SLIDE_RIGHT:
	case AnimationAttr::UNDO_SLIDE_RIGHT:
	case AnimationAttr::BOUNCE_LEFT:
	case AnimationAttr::BOUNCE_RIGHT:
		attr().set_motion_total( attr().drawing_motion_total() * d.w / 1000 );
		break;
	};


	sg_animate_init(pattr(),
			attr().type(),
			attr().path(),
			attr().step_total(),
			attr().motion_total(),
			p,
			d);

}


bool Animation::exec(void (*draw)(void *, int, int), void * obj){
	//while(App::display().animate_frame(&App::animate(), pattr(), draw, obj) > 0){
	//	;
	//}
	return false;
}

