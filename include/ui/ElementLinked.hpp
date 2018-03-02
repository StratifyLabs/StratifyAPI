/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_ELEMENTLINKED_HPP_
#define UI_ELEMENTLINKED_HPP_

#include "../draw/Animation.hpp"
#include "Element.hpp"

namespace ui {

/*! \brief Linked Element
 * \details The ElementLinked class uses links elements
 * together to create navigation options between the elements.
 *
 */
class ElementLinked : public Element {
public:

	/*! \details Constructs a Linked  Element with parent and child set to zero. */
	ElementLinked();

	/*! \details Constructs a linked element.
	 *
	 * @param parent The parent of the element
	 * @param child The child of the element
	 */
	ElementLinked(ElementLinked * parent, ElementLinked * child = 0);

	/*! \details Accesses the parent element. */
	inline ElementLinked * parent() const { return m_parent; }

	/*! \details Sets the element's parent. */
	inline void set_parent(ElementLinked * p){ m_parent = p; }

	/*! \details Accesses the element's child (0 if there is no child). */
	inline virtual ElementLinked * child() const { return m_child; }

	/*! \details Sets the element's child. */
	inline void set_child(ElementLinked * c){ m_child = c; }

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

private:
	ElementLinked * m_parent;
	ElementLinked * m_child;

	draw::Animation m_scroll_animation;
};

}

#endif /* UI_ELEMENTLINKED_HPP_ */
