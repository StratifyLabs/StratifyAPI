/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_LIST_HPP_
#define UI_LIST_HPP_


#include "ListAttr.hpp"
#include "../draw/Animation.hpp"
#include "../sys/Timer.hpp"
#include "ElementLinked.hpp"

namespace ui {

/*! \brief List Class
 * \ingroup list
 * \details The List class makes a list of
 * elements that are drawn in a vertical list.  This
 * is the primary Element used in menus.
 */
class List : public ElementLinked, public ListAttr {
public:
	List(ElementLinked * parent = 0);

	/*! \brief Return a points to item \a i in the list */
	virtual ElementLinked * at(list_attr_size_t i) = 0;

	/*! \brief Return a pointer to the currently selected item */
	inline ElementLinked * current(){ return at(selected()); }

	virtual Element * event_handler(int event, const draw::DrawingAttr & attr);

	virtual void draw(const draw::DrawingAttr & attr);
	virtual void draw_to_scale(const draw::DrawingScaledAttr & attr);
	void draw_item_to_scale(const draw::DrawingScaledAttr & attr, sg_size_t x_offset, list_attr_size_t item);

protected:
	void init(void);

	void animate_scroll(i8 dir);
	draw::Animation m_scroll_animation;
	list_attr_size_t m_draw_animation_item;
	list_attr_size_t m_draw_animation_offset;
	i8 m_select_top_bottom;
	sys::Timer m_scroll_timer;

private:
	draw::drawing_size_t m_attr_h;

};

//list of items of the same type
template<typename type, int n_items> class ListTemplate : public List {
public:
	ListTemplate(ElementLinked * parent = 0) : List(parent){}
	Element * at(list_attr_size_t i){ return m_items + i; }
	list_attr_size_t size() const { return n_items; }
	type * at_item(size_t i){ return m_items + i; }
private:
	type m_items[n_items];
};

};



#endif /* UI_LIST_HPP_ */
