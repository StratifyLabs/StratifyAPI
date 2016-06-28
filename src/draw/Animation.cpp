/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw/Animation.hpp"
#include "sys.hpp"
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
	m_drawing_attr = 0;
}

void Animation::init(Drawing * current,
		Drawing * target,
		const DrawingAttr & drawing_attr){


	m_drawing_attr = &drawing_attr;

	if( current != 0 ){
		while( m_drawing_attr->bitmap().busy() == true ){
			//wait
			Timer::wait_msec(1);
		}
		current->draw(*m_drawing_attr);
		m_drawing_attr->bitmap().refresh();
	}

	//draw target on animation screen
	if( target != 0 ){
		target->draw_scratch(*m_drawing_attr);
	}

	update_motion_total();
}

void Animation::update_motion_total(){

	sg_dim_t d;
	sg_point_t p;

	d = Drawing::dim_on_bitmap( *m_drawing_attr );
	p = Drawing::point_on_bitmap( *m_drawing_attr );

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
	while(animate_frame(draw, obj) > 0){
		;
	}
	return false;
}

int Animation::animate_frame(void (*draw)(void*,int,int), void * obj){
	const u16 delay = 18;
	int ret = 0;


	if( draw ){
		draw(obj, attr().data()->path.step, attr().data()->path.step_total);
	}

	ret = sg_animate(m_drawing_attr->bitmap().bmap(),
			m_drawing_attr->scratch()->bmap(),
			attr().data());

	m_drawing_attr->bitmap().refresh();

	Timer::wait_msec(delay);

	if( ret == 0 ){ //check for last frame
		while( m_drawing_attr->bitmap().busy() ){
			Timer::wait_msec(1);
		}
	}

	return ret;
}



