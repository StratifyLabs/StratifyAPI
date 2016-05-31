/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSBAR_HPP_
#define DRAW_PROGRESSBAR_HPP_

#include "ProgressObject.hpp"

namespace draw {


/*! \brief Progress Bar Class
 * \ingroup element
 * \details The Progress Class displays a progress bar on the screen.
 */
class ProgressBar : public ProgressObject {
public:
	ProgressBar();

	/*! \details Draws a scaled version of the progress bar within \a attr */
	void draw_to_scale(const DrawingScaledAttr & attr);

};

};

#endif /* DRAW_PROGRESSBAR_HPP_ */
