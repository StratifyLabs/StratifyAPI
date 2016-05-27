/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */


#ifndef UI_ELEMENT_HPP_
#define UI_ELEMENT_HPP_


#include <cstring>

#include "../draw/Drawing.hpp"
#include "../sgfx/Bitmap.hpp"

namespace ui {


class ElementAttr {
public:
	sgfx::Bitmap * display(){ return m_display; }
	sgfx::Bitmap * scratch(){ return m_scratch; }
	draw::DrawingAttr & drawing_attr(){ return m_drawing_attr; }

private:
	draw::DrawingAttr m_drawing_attr;
	sgfx::Bitmap * m_display;
	sgfx::Bitmap * m_scratch;

};

/*! \brief Element Class
 * \ingroup element
 * \details An Element is a basic building block of the UI.  Elements are
 * designed to be scalable so they can be adjusted to different screen sizes
 * so that Apps based on elements are future proof.
 *
 */
class Element : public draw::Drawing {
public:

	/*! \brief Construct an element with a parent */
	Element();

	/*! \brief Returns true if element is enabled */
	inline bool enabled() const { return flag(FLAG_ENABLED); }
	inline void set_enabled(bool v = true){ set_flag(FLAG_ENABLED, v); }

	/*! \brief Returns true if element is visible */
	inline bool cancelled() const { return flag(FLAG_CANCELLED); }
	inline void set_cancel(bool v = true){ set_flag(FLAG_CANCELLED, v); }

	inline bool confirmed() const { return flag(FLAG_CONFIRMED); }
	inline void set_confirm(bool v = true){ set_flag(FLAG_CONFIRMED, v); }

	/*! \brief Returns true if element's scroll is visible */
	inline bool scroll_visible() const { return flag(FLAG_SCROLL_VISIBLE); }
	inline void set_scroll_visible(bool v = true){ set_flag(FLAG_SCROLL_VISIBLE, v); }

	inline bool exclusive_checklist() const { return flag(FLAG_EXCLUSIVE_CHECKLIST); }
	inline void set_exclusive_checklist(bool v = true){ set_flag(FLAG_EXCLUSIVE_CHECKLIST, v); }

	virtual void set_animation_type(u8 v);
	virtual void set_animation_path(u8 v);
	void set_animation(u8 type, u8 path);
	virtual u8 animation_type() const;
	virtual u8 animation_path() const;

	/*! \brief Element events */
	enum {
		ENTER /*! \brief Enter the element (ie, transition to the Element) */,
		UPDATE /*! \brief Update the element (use App::set_update_period() to set frequency of execution) */,
		SETUP /*! \brief Setup the element (called at program startup */,
		EVENT_TOTAL
	};


	/*! \brief Element event handler
	 *
	 * \details This method is called on the current Element when an event (e.g., Element::ENTER,
	 * Element::UPDATE, Element::TOP_PRESS) happens.
	 *
	 * @param event The event to execute
	 * @return For transitions, the new element is returned; otherwise this
	 */
	virtual Element * event_handler(int event, const draw::DrawingAttr & attr);

	//void set_dim(sg_size_t w, sg_size_t h);

protected:

	enum {
		FLAG_ENABLED = FLAG_DRAWING_TOTAL,
		FLAG_EXCLUSIVE_CHECKLIST,
		FLAG_SCROLL_VISIBLE,
		FLAG_BUSY,
		FLAG_CANCELLED,
		FLAG_CONFIRMED,
		FLAG_TOTAL
	};

	virtual void draw_scroll(const draw::DrawingScaledAttr & attr, int selected, int total, int visible);
	void adjust_x_center(sg_size_t w, sg_int_t & x);

private:

	//ElementAttr _attr;

};

};

#endif /* UI_ELEMENT_HPP_ */
