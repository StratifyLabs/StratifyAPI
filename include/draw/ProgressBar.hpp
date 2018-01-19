/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSBAR_HPP_
#define DRAW_PROGRESSBAR_HPP_

#include "Progress.hpp"

namespace draw {


/*! \brief Progress Bar Class
 * \ingroup element
 * \details The Progress Class displays a progress bar on the screen.
 */
class ProgressBar : public Progress {
public:
	ProgressBar();

	virtual void draw(const DrawingAttr & attr){
		drawing_size_t thickness = attr.calc_height(border_thickness());
		m_scaled_border_thickness = attr.calc_height_on_bitmap(thickness);
		Drawing::draw(attr);
	}

	void draw_to_scale(const DrawingScaledAttr & attr);

};

};

#endif /* DRAW_PROGRESSBAR_HPP_ */
