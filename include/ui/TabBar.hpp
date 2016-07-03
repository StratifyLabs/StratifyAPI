/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TABBAR_HPP_
#define UI_TABBAR_HPP_

#include "../sys/Timer.hpp"
#include "../draw/Drawing.hpp"
#include "../sgfx/Bitmap.hpp"
#include "../draw/Animation.hpp"
#include "Tab.hpp"
#include "ListAttr.hpp"


namespace ui {

/*! \brief Tab Bar Class
 * \details This class is used for UI navigation using a Tab bar.
 */
class TabBar : public Element, public ListAttr {
public:

	/*! \details Tab Bar construction */
	TabBar();

	/*! \details Access the height of the tab bar as a ratio to Drawing::scale() */
	sg_size_t h() const { return m_height; }

	/*! \details Set the height of the tab bar where Drawing::scale() height will set the height
	 * to fill the entire screen.
	 *
	 * @param h Height of the tab bar as a ratio to Drawing::scale()
	 *
	 */
	void set_height(sg_size_t h){ m_height = h; }

	sg_size_t highlight() const { return m_highlight; }
	void set_highlight(sg_size_t h){ m_highlight = h; }

	Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);
	void draw(const draw::DrawingAttr & attr);

	/*! \details This method access a refernence
	 * to the tab at the specified offset.  This
	 * method must be implemented by a class that inherits
	 * ui::TabBar.
	 *
	 * @param i The offset of the tab to access
	 * @return A reference to the tab at the specified offset.
	 */
	virtual Tab & at(list_attr_size_t i) = 0;
	Tab & current(){ return at(selected()); }

	void set_animation_type(u8 v);
	u8 animation_type() const;
	void set_animation_path(u8 v);
	u8 animation_path() const;

	draw::Animation & animation(){ return m_animation; }

protected:
	void draw_tab_bar(const draw::DrawingAttr & attr, int selected);

private:

	draw::drawing_size_t m_height;
	draw::drawing_size_t m_highlight;

	mutable draw::Animation m_animation;

	void scroll(int dir, bool repeat, const draw::DrawingAttr & attr);
	bool m_bounced;

	sg_size_t tab_selected_offset();
	void set_animate_bounce(int type);
	void set_animate_push(int type, bool repeat);

};

};

#endif /* UI_TABBAR_HPP_ */
