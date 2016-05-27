/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TABBAR_HPP_
#define UI_TABBAR_HPP_

#include "ListAttr.hpp"
#include "../draw/Drawing.hpp"
#include "../sgfx/Bitmap.hpp"
#include "../draw/Animation.hpp"
#include "Tab.hpp"


namespace ui {

class TabBar : public Element, public ListAttr {
public:
	TabBar();

	sg_size_t h() const { return m_height; }
	void set_height(sg_size_t h){ m_height = h; }

	sg_size_t highlight() const { return m_highlight; }
	void set_highlight(sg_size_t h){ m_highlight = h; }

	Element * event_handler(int event, const draw::DrawingAttr & attr);
	void draw(const draw::DrawingAttr & attr);

	//Return a point to the Tab at \a i
	virtual Tab * at(list_attr_size_t i) = 0;

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

	int animate_frame(sgfx::Bitmap * bitmap, sg_animation_t * animation);

};

};

#endif /* UI_TABBAR_HPP_ */
