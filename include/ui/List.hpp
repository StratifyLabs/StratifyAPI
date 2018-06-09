/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_LIST_HPP_
#define UI_LIST_HPP_


#include "../draw/Animation.hpp"
#include "../chrono/Timer.hpp"
#include "LinkedElement.hpp"
#include "ListAttr.hpp"

namespace ui {

/*! \brief List Class
 * \ingroup list
 * \details The List class makes a list of
 * elements that are drawn in a vertical list.  This
 * is the primary Element used in menus.
 */
class List : public LinkedElement, public ListAttr {
public:
	List(LinkedElement * parent = 0);

	/*! \details Return a points to item \a i in the list */
	virtual LinkedElement & at(list_attr_size_t i) = 0;

	/*! \details Return a pointer to the currently selected item */
	inline LinkedElement & current(){ return at(selected()); }

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

	virtual void draw_to_scale(const draw::DrawingScaledAttr & attr);
	void draw_item_to_scale(const draw::DrawingScaledAttr & attr, sg_size_t x_offset, list_attr_size_t item);

protected:
	void init(void);

	void handle_down_button_actuation(const ui::Event  & event, const draw::DrawingAttr & attr);
	void handle_up_button_actuation(const ui::Event  & event, const draw::DrawingAttr & attr);
	void handle_select_button_actuation(const ui::Event  & event, const draw::DrawingAttr & attr);

	void animate_scroll(i8 dir, const draw::DrawingAttr & attr);
	draw::Animation m_scroll_animation;
	list_attr_size_t m_draw_animation_item;
	list_attr_size_t m_draw_animation_offset;
	i8 m_select_top_bottom;
    chrono::Timer m_scroll_timer;

private:

};

//list of items of the same type
template<typename type, int n_items> class ListTemplate : public List {
public:
	ListTemplate(LinkedElement * parent = 0) : List(parent){}
	Element * at(list_attr_size_t i){ return m_items + i; }
	list_attr_size_t size() const { return n_items; }
	type * at_item(u32 i){ return m_items + i; }
private:
	type m_items[n_items];
};

}



#endif /* UI_LIST_HPP_ */
