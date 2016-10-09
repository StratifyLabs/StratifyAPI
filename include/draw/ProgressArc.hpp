/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSARC_HPP_
#define DRAW_PROGRESSARC_HPP_

#include "Progress.hpp"

namespace draw {

/*! \brief Progress Arc Class
 * \ingroup element
 * \details The Progress Class displays a progress arc on the screen.
 */
class ProgressArc : public Progress {
public:
	ProgressArc();

	/*! \details Draw a scaled version of the arc using the specified \a attr */
	void draw_to_scale(const DrawingScaledAttr & attr);

	void set_direction(s8 dir){
		m_direction = dir;
	}

	void set_offset(s16 offset){
		m_offset = offset;
	}


private:
	s16 m_offset;
	s8 m_direction;
};

};

#endif /* DRAW_PROGRESSARC_HPP_ */
