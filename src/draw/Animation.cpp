/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "draw/Animation.hpp"
#include "sys.hpp"
#include "chrono.hpp"

using namespace draw;

AnimationAttr::AnimationAttr(){
	attr().type = PUSH_LEFT;
	attr().path.type = SQUARED;
	attr().path.step_total = 10;
	attr().path.motion_total = 100;
	m_drawing_motion_total = 1000;
	m_frame_delay = 18;
}

Animation::Animation() {
	// TODO Auto-generated constructor stub
	m_drawing_attr = 0;
}

Animation::Animation(const AnimationAttr & attr){
	m_drawing_attr = 0;
	assign(attr);
}

void Animation::init(Drawing * current,
							Drawing * target,
							const DrawingAttr & drawing_attr){


	m_drawing_attr = &drawing_attr;

	if( current != 0 ){
		m_drawing_attr->bitmap().wait(chrono::MicroTime(1000));
		current->draw(drawing_attr);
		m_drawing_attr->bitmap().refresh();
	}

	//draw target on animation screen
	if( target != 0 ){
		target->draw_scratch(drawing_attr);
	}

	reinit();
}

void Animation::reinit(){

	sg_area_t d;
	sg_point_t p;
	drawing_size_t motion;

	if( m_drawing_attr ){

		p = m_drawing_attr->calc_point_on_bitmap();
		d = m_drawing_attr->calc_dim_on_bitmap();

		//convert motion total
		switch(type()){
			case AnimationAttr::PUSH_DOWN:
			case AnimationAttr::PUSH_UP:
			case AnimationAttr::SLIDE_DOWN:
			case AnimationAttr::UNDO_SLIDE_DOWN:
			case AnimationAttr::SLIDE_UP:
			case AnimationAttr::UNDO_SLIDE_UP:
			case AnimationAttr::BOUNCE_UP:
			case AnimationAttr::BOUNCE_DOWN:
				//convert motion total as a height
				if( drawing_motion_total() > m_drawing_attr->height() ){
					motion = m_drawing_attr->height();
				} else {
					motion = drawing_motion_total();
				}
				set_motion_total( motion * d.height / m_drawing_attr->height() );
				break;
			case AnimationAttr::PUSH_LEFT:
			case AnimationAttr::PUSH_RIGHT:
			case AnimationAttr::SLIDE_LEFT:
			case AnimationAttr::UNDO_SLIDE_LEFT:
			case AnimationAttr::SLIDE_RIGHT:
			case AnimationAttr::UNDO_SLIDE_RIGHT:
			case AnimationAttr::BOUNCE_LEFT:
			case AnimationAttr::BOUNCE_RIGHT:
				if( drawing_motion_total() > m_drawing_attr->width() ){
					motion = m_drawing_attr->width();
				} else {
					motion = drawing_motion_total();
				}
				set_motion_total( motion * d.width / m_drawing_attr->width() );
				break;
		};

		api()->animate_init(pattr(),
									  type(),
									  path(),
									  step_total(),
									  motion_total(),
									  p,
									  d);

	}

}


bool Animation::exec(void (*draw)(void *, int, int), void * obj){
	m_drawing_attr->bitmap().wait(MicroTime(1000));
	while(animate_frame(draw, obj) > 0){
		;
	}
	return false;
}

int Animation::animate_frame(void (*draw)(void*,int,int), void * obj){
	int ret = 0;
	enum sgfx::Pen::flags o_flags;

	if( draw ){
		draw(obj, data()->path.step, data()->path.step_total);
	}

	o_flags = m_drawing_attr->bitmap().pen().o_flags();

	m_drawing_attr->bitmap() << sgfx::Pen().set_flags(sgfx::Pen::IS_SOLID);

	ret = api()->animate(m_drawing_attr->bitmap().bmap(),
									m_drawing_attr->scratch()->bmap(),
									data());

	m_drawing_attr->bitmap() << sgfx::Pen().set_flags(o_flags);
	m_drawing_attr->bitmap().refresh();
	chrono::wait(Milliseconds(frame_delay()));

	m_drawing_attr->bitmap().wait(Microseconds(1000));

	return ret;
}



