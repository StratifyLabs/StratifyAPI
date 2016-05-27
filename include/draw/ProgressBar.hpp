/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSBAR_HPP_
#define DRAW_PROGRESSBAR_HPP_

#include <draw/ProgressObject.hpp>

namespace draw {


/*! \brief Progress Bar Class
 * \ingroup element
 * \details The Progress Class displays a progress bar on the screen.
 */
class ProgressBar : public ProgressObject {
public:
	ProgressBar();
	ProgressBar(u16 value, u16 max);

	void draw_to_scale(const DrawingScaledAttr & attr);

};

};

#endif /* DRAW_PROGRESSBAR_HPP_ */
