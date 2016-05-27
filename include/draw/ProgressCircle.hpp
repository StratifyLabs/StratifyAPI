/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSCIRCLE_HPP_
#define DRAW_PROGRESSCIRCLE_HPP_

#include <draw/ProgressObject.hpp>

namespace draw {

/*! \brief Progress Circle Class
 * \ingroup element
 * \details The Progress Class displays a progress circle on the screen.
 */
class ProgressCircle : public ProgressObject {
public:
	ProgressCircle();
	ProgressCircle(u16 value, u16 max);

private:
	void draw_to_scale(const DrawingScaledAttr & attr);
};

};

#endif /* DRAW_PROGRESSCIRCLE_HPP_ */
