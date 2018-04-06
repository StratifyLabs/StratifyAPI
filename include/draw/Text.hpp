/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXT_HPP_
#define DRAW_TEXT_HPP_

#include "Drawing.hpp"
#include "TextAttr.hpp"

namespace draw {


/*! \brief Text Class
 * \details This is a text label.  The object automatically
 * chooses the correct font height to fit within the area
 * specified.
 *
 * In order for this class to work correctly, sys::Assets::init() must
 * be invoked so that the application is aware of the system fonts.
 *
 */
class Text : public Drawing, public TextAttr {
public:
	/*! \details Construct a label with text */
	Text(const char * text = 0);

	/*! \details Draw the scaled text as specified by \a attr */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

private:

};

}

#endif /* DRAW_TEXT_HPP_ */
