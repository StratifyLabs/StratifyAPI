/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSARC_HPP_
#define DRAW_PROGRESSARC_HPP_

#include <draw/ProgressObject.hpp>

namespace draw {

/*! \brief Progress Arc Class
 * \ingroup element
 * \details The Progress Class displays a progress arc on the screen.
 */
class ProgressArc : public ProgressObject {
public:
	ProgressArc();
	ProgressArc(u16 value, u16 max);

	void draw_to_scale(const DrawingScaledAttr & attr);
};

};

#endif /* DRAW_PROGRESSARC_HPP_ */
