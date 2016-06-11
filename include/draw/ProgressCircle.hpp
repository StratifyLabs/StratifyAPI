/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSCIRCLE_HPP_
#define DRAW_PROGRESSCIRCLE_HPP_

#include "Progress.hpp"

namespace draw {

/*! \brief Progress Circle Class
 * \ingroup element
 * \details The Progress Class displays a progress circle on the screen.
 */
class ProgressCircle : public Progress {
public:
	ProgressCircle();

	/*! \details Draw a scaled version of the circle as specified by \a attr */
	void draw_to_scale(const DrawingScaledAttr & attr);

private:
};

};

#endif /* DRAW_PROGRESSCIRCLE_HPP_ */
