/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_ANIMATION_HPP_
#define DRAW_ANIMATION_HPP_

#include "Drawing.hpp"

namespace draw {

/*! \brief Animation Attributes Class
 */
class AnimationAttr : public api::DrawInfoObject {
public:

	/*! \details Constructs a new animation attribute object. */
	AnimationAttr();

	enum {
		PUSH_LEFT /*! Push Left */ = SG_ANIMATION_TYPE_PUSH_LEFT,
		PUSH_RIGHT /*! Push Right */ = SG_ANIMATION_TYPE_PUSH_RIGHT,
		PUSH_UP /*! Push Up */ = SG_ANIMATION_TYPE_PUSH_UP,
		PUSH_DOWN /*! Push Down */ = SG_ANIMATION_TYPE_PUSH_DOWN,
		SLIDE_LEFT /*! Slide Left */ = SG_ANIMATION_TYPE_SLIDE_LEFT,
		UNDO_SLIDE_LEFT = SG_ANIMATION_TYPE_UNDO_SLIDE_LEFT,
		SLIDE_UP /*! Slide Up */ = SG_ANIMATION_TYPE_SLIDE_UP,
		UNDO_SLIDE_UP /*! Undo Slide up */ = SG_ANIMATION_TYPE_UNDO_SLIDE_UP,
		SLIDE_RIGHT /*! Slide Right */ = SG_ANIMATION_TYPE_SLIDE_RIGHT,
		UNDO_SLIDE_RIGHT = SG_ANIMATION_TYPE_UNDO_SLIDE_RIGHT,
		SLIDE_DOWN /*! Slide Down */ = SG_ANIMATION_TYPE_SLIDE_DOWN,
		UNDO_SLIDE_DOWN /*! Undo Slide Down */ = SG_ANIMATION_TYPE_UNDO_SLIDE_DOWN,
		NONE /*! No Animation */ = SG_ANIMATION_TYPE_NONE,
		BOUNCE_UP /*! Bounce off the top when scrolling up */ = SG_ANIMATION_TYPE_BOUNCE_UP,
		BOUNCE_DOWN /*! Bounce off the bottom when scrolling down */ = SG_ANIMATION_TYPE_BOUNCE_DOWN,
		BOUNCE_LEFT /*! Bounce off the left side */ = SG_ANIMATION_TYPE_BOUNCE_LEFT,
		BOUNCE_RIGHT /*! Bounce off the right side */ = SG_ANIMATION_TYPE_BOUNCE_RIGHT
	};

	enum {
		LINEAR /*! Linear Animation */ = SG_ANIMATION_PATH_LINEAR,
		SQUARED /*! Accelerating animation */ = SG_ANIMATION_PATH_SQUARED,
		SQUARED_UNDO /*! De-accelerating animation */ = SG_ANIMATION_PATH_SQUARED_UNDO
	};


	void assign(const AnimationAttr & attr){
		m_attr = attr.m_attr;
		m_drawing_motion_total = attr.m_drawing_motion_total;
		m_frame_delay = attr.m_frame_delay;
	}

	/*! \details Returns a reference to the attibutes. */
	sg_animation_t & attr(){ return m_attr; }
	/*! \details Sets the animation type (e.g. Animation::PUSH_DOWN). */
	void set_type(u8 v){ m_attr.type = v; }
	/*! \details Sets the path (e.g. Animation::SQUARED). */
	void set_path(u8 v){ m_attr.path.type = v; }
	/*! \details Sets the total number of steps in the animation. */
	void set_step_total(u8 v){ m_attr.path.step_total = v; }
	/*! \details Sets the total amount of motion (in pixels) of the animation. */
	void set_motion_total(sg_size_t v){ m_attr.path.motion_total = v; }

	//void set_drawing_start(drawing_point_t v){ m_drawing_attr.start = v; }
	//void set_drawing_start(drawing_int_t x, drawing_int_t y){ m_drawing_attr.start.x = x; m_drawing_attr.start.y = y; }
	//void set_drawing_dim(drawing_dim_t v){ m_drawing_attr.dim = v; }
	//void set_drawing_dim(drawing_size_t w, drawing_size_t h){ m_drawing_attr.dim.width = w; m_drawing_attr.dim.height = h; }
	/*! \details Sets the total amount of motion as a ratio to the drawing scale. */
	void set_drawing_motion_total(drawing_size_t v){ m_drawing_motion_total = v; }

	/*! \details Accesses the type (e.g. Animation::PUSH_DOWN). */
	u8 type() const { return m_attr.type; }
	/*! \details Accesses the path (e.g. Animation::SQUARED). */
	u8 path() const { return m_attr.path.type; }
	/*! \details Accesses the animation step total. */
	u16 step_total() const { return m_attr.path.step_total; }
	/*! \details Accesses the animation start point (top-left corner). */
	sg_point_t start() const { return m_attr.region.point; }
	/*! \details Accesses the animation's dimension (width and height from top-left corner). */
	sg_dim_t dim() const { return m_attr.region.dim; }
	/*! \details Accesses the total motion (in pixels). */
	sg_size_t motion_total() const { return m_attr.path.motion_total; }
	/*! \details Accesses the motion total in a drawing scale. */
	drawing_size_t drawing_motion_total() const { return m_drawing_motion_total; }

	sg_animation_t * data(){ return &m_attr; }

	/*! \details Accesses the frame delay of the animation. */
	u16 frame_delay() const { return m_frame_delay; }

	/*! \details Sets the frame delay (in ms) of the animation.
	 *
	 * @param value The number of milliseconds to wait between frames.
	 *
	 * The animation will at least wait the amount of time required
	 * to refresh the display. This delay will only have an effect
	 * if it is longer than the time required to update the display.
	 */
	void set_frame_delay(u16 value){
		m_frame_delay = value;
	}

private:
	sg_animation_t m_attr;
	drawing_size_t m_drawing_motion_total;
	u16 m_frame_delay;
};

/*! \brief Animation Class
 */
class Animation : public AnimationAttr, public api::DrawWorkObject {
public:

	/*! \details Constructs a new animation object. */
	Animation();
	Animation(const AnimationAttr & attr);

	/*! \details Initializes the animation.
	 *
	 * @param current The current drawing
	 * @param target The target drawing
	 * @param attr The drawing attributes
	 *
	 * If \a current is non-null it will be drawn on the visible screen
	 * when init() is called.
	 *
	 * If \a target is non-null, it will be drawn on the scratch bitmap. Most animations
	 * will transition from the \a current Drawing to the \a target drawing.
	 *
	 * The drawing attributes (\a attr) will be used to define the starting point and
	 * dimensions of the animation attributes.
	 *
	 */
	void init(Drawing * current,
			Drawing * target,
			const DrawingAttr & attr);

	void reinit();


	/*! \details Executes the animation.
	 *
	 * @param draw A callback that is execute between each frame
	 * @param obj The argument passed to the draw callback
	 *
	 * The draw callback can be used to update the display. This will
	 * work best on areas of the screen that are not affected by the animation.
	 *
	 *
	 */
	bool exec(void (*draw)(void*,int,int) = 0, void * obj = 0);

private:
	int animate_frame(void (*draw)(void*,int,int), void * obj);
	sg_animation_t * pattr(){ return data(); }
	const DrawingAttr * m_drawing_attr;

};

}

#endif /* DRAW_ANIMATION_HPP_ */
