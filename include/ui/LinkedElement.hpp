/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SAPI_UI_LINKEDELEMENT_HPP_
#define SAPI_UI_LINKEDELEMENT_HPP_

#include "../draw/Animation.hpp"
#include "Element.hpp"

namespace ui {

/*! \brief Linked Element
 * \details The Linked Element class uses links elements
 * together to create navigation options between the elements.
 *
 */
class LinkedElement : public Element {
public:

	/*! \details Constructs a Linked  Element with parent and child set to zero. */
	LinkedElement();

	/*! \details Constructs a linked element.
	 *
	 * @param parent The parent of the element
	 * @param child The child of the element
	 */
	LinkedElement(LinkedElement * parent, LinkedElement * child = 0);

	/*! \details Accesses the parent element. */
	inline LinkedElement * parent() const { return m_parent; }

	/*! \details Sets the element's parent. */
	inline void set_parent(LinkedElement * p){ m_parent = p; }

	/*! \details Accesses the element's child (0 if there is no child). */
	inline virtual LinkedElement * child() const { return m_child; }

	/*! \details Sets the element's child. */
	inline void set_child(LinkedElement * c){ m_child = c; }

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

private:
	friend class ElementLinked;
	LinkedElement * m_parent;
	LinkedElement * m_child;

	draw::Animation m_scroll_animation;
};

}

#endif /* SAPI_UI_LINKEDELEMENT_HPP_ */
