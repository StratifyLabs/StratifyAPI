/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_CIRCLEPROGRESS_HPP_
#define SAPI_DRAW_CIRCLEPROGRESS_HPP_

#include "Progress.hpp"

namespace draw {

/*! \brief Progress Circle Class
 * \ingroup element
 * \details The Progress Class displays a progress circle on the screen.
 */
class CircleProgress : public Progress {
public:
	CircleProgress();

	/*! \details Draw a scaled version of the circle as specified by \a attr */
	void draw_to_scale(const DrawingScaledAttr & attr);

private:
};

}

#endif /* SAPI_DRAW_CIRCLEPROGRESS_HPP_ */
