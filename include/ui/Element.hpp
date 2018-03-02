/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_ELEMENT_HPP_
#define UI_ELEMENT_HPP_


#include <cstring>

#include "../draw/Drawing.hpp"
#include "../draw/Animation.hpp"
#include "../sgfx/Bitmap.hpp"
#include "Event.hpp"

namespace ui {

class EventLoop;

/*! \brief Element Class
 * \details An Element is a basic building block of the UI.  Elements are
 * designed to be scalable so they can be adjusted to different screen sizes
 * so that Apps based on elements are future proof.
 *
 */
class Element : public draw::Drawing {
public:

	/*! \details Construct an element */
	Element();

	/*! \details Returns true if element is enabled */
	inline bool is_enabled() const { return flag(FLAG_ENABLED); }
	/*! \details Enable the element */
	inline void set_enable(bool v = true){ set_flag(FLAG_ENABLED, v); }
	inline void set_enabled(bool v = true){ set_flag(FLAG_ENABLED, v); }

	/*! \details Returns true if element is visible */
	inline bool is_cancelled() const { return flag(FLAG_CANCELLED); }
	inline void set_cancel(bool v = true){ set_flag(FLAG_CANCELLED, v); }

	inline bool is_confirmed() const { return flag(FLAG_CONFIRMED); }
	inline void set_confirm(bool v = true){ set_flag(FLAG_CONFIRMED, v); }

	/*! \details Returns true if element's scroll is visible */
	inline bool is_scroll_visible() const { return flag(FLAG_SCROLL_VISIBLE); }
	inline void set_scroll_visible(bool v = true){ set_flag(FLAG_SCROLL_VISIBLE, v); }

	/*! \details Access whether bouncing left is enabled */
	bool is_bounce_left_enabled() const { return flag(FLAG_BOUNCE_LEFT_ENABLE); }
	/*! \details Set whether the tab should bounce on trying to go left on the left-most tab */
	void set_bounce_left_enabled(bool v = true){ set_flag(FLAG_BOUNCE_LEFT_ENABLE, v); }

	/*! \details Access whether bouncing right is enabled */
	bool is_bounce_right_enabled() const { return flag(FLAG_BOUNCE_RIGHT_ENABLE); }
	/*! \details Set whether the tab should bounce on trying to go right on the right-most tab */
	void set_bounce_right_enabled(bool v = true){ set_flag(FLAG_BOUNCE_RIGHT_ENABLE, v); }

	/*! \details This methods returns true if the handle_event()
	 * called the drawidth() method and the underlying device needs to be
	 * redrawn
	 *
	 * @return True if it is time to redraw the element on the LCD or bitmap
	 */
	inline bool is_redraw_pending() const { return flag(FLAG_REDRAW_PENDING); }

	/*! \details This methods sets whether a redraw is pending.  A control
	 * loop that handles the element might look like this:
	 *
	 * \code
	 *  LCD lcd; //lcd that inherits Bitmap
	 *  DrawingAttr drawing_attr; //holds the info about the bitmap and
	 *  drawing_attr.set(lcd, drawing_point(0,0), drawing_dim(1000, 1000)); //full screen element
	 *  My element; //my special element that inherits Element and re-implements the handle_event() and drawidth() methods
	 * 	while(1){
	 * 		element.handle_event(Element::UPDATE, drawing_attr);
	 * 		if( element.is_redraw_pending() ){
	 * 			lcd.refresh(); //copy the bitmap to the hardware device -- redraw
	 * 			element.set_redraw_pending(false); //clear the redraw flag
	 * 		}
	 *		Timer::wait_msec(100);  //update loop once every ten seconds
	 * 	}
	 *
	 * \endcode
	 *
	 * @param v The value of the redraw pending flags
	 */
	inline void set_redraw_pending(bool v = true){ set_flag(FLAG_REDRAW_PENDING, v); }

	inline bool is_exclusive_checklist() const { return flag(FLAG_EXCLUSIVE_CHECKLIST); }
	inline void set_exclusive_checklist(bool v = true){ set_flag(FLAG_EXCLUSIVE_CHECKLIST, v); }


	draw::AnimationAttr & animation_attr(){ return m_animation_attr; }

	void set_animation_type(u8 v){ m_animation_attr.set_type(v); }
	void set_animation_path(u8 v){ m_animation_attr.set_path(v); }
	void set_animation(u8 type, u8 path, u16 delay = (u16)-1);
	u8 animation_type() const { return m_animation_attr.type(); }
	u8 animation_path() const { return m_animation_attr.path(); }

	void set_animation_frame_delay(u16 delay){ m_animation_attr.set_frame_delay(delay); }
	u16 animation_frame_delay() const { return m_animation_attr.frame_delay(); }


	/*! \details This method is called on the current Element when an event (e.g., Element::ENTER,
	 * Element::UPDATE, Element::TOP_PRESS) happens.
	 *
	 * @param event The event to execute
	 * @param attr The drawing attributes to use for the event
	 * @return For transitions, the new element is returned; otherwise this
	 */
	virtual Element * handle_event(const Event & event, const draw::DrawingAttr & attr);


    /*! \details Returns a pointer to the element's event loop.
     *
     * If the element isn't running in an event loop, this
     * method will return 0.
     *
     */
    EventLoop * event_loop() const { return m_event_loop; }

protected:

	enum {
		FLAG_ENABLED = FLAG_DRAWING_TOTAL,
		FLAG_REDRAW_PENDING,
		FLAG_EXCLUSIVE_CHECKLIST,
		FLAG_SCROLL_VISIBLE,
		FLAG_BUSY,
		FLAG_CANCELLED,
		FLAG_CONFIRMED,
		FLAG_BOUNCE_LEFT_ENABLE,
		FLAG_BOUNCE_RIGHT_ENABLE,
		FLAG_ELEMENT_TOTAL
	};


	draw::AnimationAttr m_animation_attr;

	virtual void draw_scroll(const draw::DrawingScaledAttr & attr, int selected, int total, int visible);
	void adjust_x_center(sg_size_t w, sg_int_t & x);

    friend class EventLoop;
    void set_event_loop(EventLoop * event_loop){ m_event_loop = event_loop; }

private:

    EventLoop * m_event_loop;

};

}

#endif /* UI_ELEMENT_HPP_ */
