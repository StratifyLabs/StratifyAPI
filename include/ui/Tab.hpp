/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SAPI_UI_TAB_HPP_
#define SAPI_UI_TAB_HPP_


#include "Element.hpp"
#include "../draw/Drawing.hpp"

namespace ui {

/*! \brief Tab Class
 * \details This method draws a Tab that is a part of ui::TabBar.
 */
class Tab : public draw::Drawing {
public:

	/*! \details Contruct an emtpy tab */
	Tab();

	/*! \details Assign an element to be associated with this tab. */
	void set_element(Element * element){ m_element = element; }

	/*! \details Access a pointer to the element associated with this tab. */
	Element * element(){ return m_element; }

	/*! \details Calculate a square within the tab */
	static void calc_square(draw::drawing_point_t & point, draw::drawing_area_t & dim);

protected:

private:
	Element * m_element;
};

}

#endif /* SAPI_UI_TAB_HPP_ */
