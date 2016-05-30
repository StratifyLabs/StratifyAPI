/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_ELEMENTLINKED_HPP_
#define UI_ELEMENTLINKED_HPP_

#include "../draw/Animation.hpp"
#include "Element.hpp"

namespace ui {

class ElementLinked : public Element {
public:
	ElementLinked(ElementLinked * parent, ElementLinked * child = 0);

	/*! \brief Return the parent element */
	inline ElementLinked * parent() const { return m_parent; }

	/*! \brief Set the element's parent */
	inline void set_parent(ElementLinked * p){ m_parent = p; }

	/*! \brief Return the element's child (0 if there is no child) */
	inline virtual ElementLinked * child() const { return m_child; }

	/*! \brief Set the element's child */
	inline void set_child(ElementLinked * c){ m_child = c; }

	virtual Element * event_handler(int event, const draw::DrawingAttr & attr);

	u8 animation_type() const { return m_animation_type; }
	void set_animation_type(u8 v){ m_animation_type = v; }


private:
	u8 m_animation_type;
	ElementLinked * m_parent;
	ElementLinked * m_child;

	draw::Animation m_scroll_animation;
};

};

#endif /* UI_ELEMENTLINKED_HPP_ */
