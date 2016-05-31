/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSARC_HPP_
#define DRAW_PROGRESSARC_HPP_

#include "ProgressObject.hpp"

namespace draw {

/*! \brief Progress Arc Class
 * \ingroup element
 * \details The Progress Class displays a progress arc on the screen.
 */
class ProgressArc : public ProgressObject {
public:
	ProgressArc();

	/*! \details Draw a scaled version of the arc using the specified \a attr */
	void draw_to_scale(const DrawingScaledAttr & attr);
};

};

#endif /* DRAW_PROGRESSARC_HPP_ */
