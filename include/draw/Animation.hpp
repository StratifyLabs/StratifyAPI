/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_ANIMATION_HPP_
#define DRAW_ANIMATION_HPP_

#include "Drawing.hpp"

namespace draw {

class AnimationAttr {
public:
	AnimationAttr();
	enum {
		PUSH_LEFT = SG_ANIMATION_TYPE_PUSH_LEFT,
		PUSH_RIGHT = SG_ANIMATION_TYPE_PUSH_RIGHT,
		PUSH_UP = SG_ANIMATION_TYPE_PUSH_UP,
		PUSH_DOWN = SG_ANIMATION_TYPE_PUSH_DOWN,
		SLIDE_LEFT = SG_ANIMATION_TYPE_SLIDE_LEFT,
		UNDO_SLIDE_LEFT = SG_ANIMATION_TYPE_UNDO_SLIDE_LEFT,
		SLIDE_UP = SG_ANIMATION_TYPE_SLIDE_UP,
		UNDO_SLIDE_UP = SG_ANIMATION_TYPE_UNDO_SLIDE_UP,
		SLIDE_RIGHT = SG_ANIMATION_TYPE_SLIDE_RIGHT,
		UNDO_SLIDE_RIGHT = SG_ANIMATION_TYPE_UNDO_SLIDE_RIGHT,
		SLIDE_DOWN = SG_ANIMATION_TYPE_SLIDE_DOWN,
		UNDO_SLIDE_DOWN = SG_ANIMATION_TYPE_UNDO_SLIDE_DOWN,
		NONE = SG_ANIMATION_TYPE_NONE,
		BOUNCE_UP = SG_ANIMATION_TYPE_BOUNCE_UP,
		BOUNCE_DOWN = SG_ANIMATION_TYPE_BOUNCE_DOWN,
		BOUNCE_LEFT = SG_ANIMATION_TYPE_BOUNCE_LEFT,
		BOUNCE_RIGHT = SG_ANIMATION_TYPE_BOUNCE_RIGHT
	};

	enum {
		LINEAR = SG_ANIMATION_PATH_LINEAR,
		SQUARED = SG_ANIMATION_PATH_SQUARED,
		SQUARED_UNDO = SG_ANIMATION_PATH_SQUARED_UNDO
	};

	sg_animation_t & attr(){ return m_attr; }
	void set_type(u8 v){ m_attr.type = v; }
	void set_path(u8 v){ m_attr.path.type = v; }
	void set_step_total(u8 v){ m_attr.path.step_total = v; }
	void set_motion_total(sg_size_t v){ m_attr.path.motion_total = v; }

	//void set_drawing_start(drawing_point_t v){ m_drawing_attr.start = v; }
	//void set_drawing_start(drawing_int_t x, drawing_int_t y){ m_drawing_attr.start.x = x; m_drawing_attr.start.y = y; }
	//void set_drawing_dim(drawing_dim_t v){ m_drawing_attr.dim = v; }
	//void set_drawing_dim(drawing_u32 w, drawing_u32 h){ m_drawing_attr.dim.w = w; m_drawing_attr.dim.h = h; }
	void set_drawing_motion_total(drawing_u32 v){ m_drawing_motion_total = v; }


	u8 type() const { return m_attr.type; }
	u8 path() const { return m_attr.path.type; }
	u16 step_total() const { return m_attr.path.step_total; }
	sg_point_t start() const { return m_attr.start; }
	sg_dim_t dim() const { return m_attr.dim; }
	sg_size_t motion_total() const { return m_attr.path.motion_total; }

	drawing_u32 drawing_motion_total() const { return m_drawing_motion_total; }

	sg_animation_t * data(){ return &m_attr; }

private:
	sg_animation_t m_attr;
	drawing_u32 m_drawing_motion_total;
};

class Animation : public AnimationAttr {
public:
	Animation();
	void init(Drawing * current,
			Drawing * target,
			const DrawingAttr & attr);
	bool exec(void (*draw)(void*,int,int) = 0, void * obj = 0);
	void update_motion_total();


private:
	int animate_frame(void (*draw)(void*,int,int), void * obj);
	sg_animation_t * pattr(){ return data(); }
	const DrawingAttr * m_drawing_attr;

};

};

#endif /* DRAW_ANIMATION_HPP_ */
