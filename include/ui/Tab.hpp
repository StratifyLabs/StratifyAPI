/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_HPP_
#define UI_TAB_HPP_


#include "Element.hpp"
#include "../draw/Drawing.hpp"

namespace ui {

class Tab : public draw::Drawing{
public:
	Tab();

	void set_element(Element * element){ m_element = element; }
	Element * element(){ return m_element; }

	static void calc_square(draw::drawing_point_t & point, draw::drawing_dim_t & dim);

protected:

private:
	Element * m_element;
};

};

#endif /* UI_TAB_HPP_ */
