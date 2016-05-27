/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXT_HPP_
#define DRAW_TEXT_HPP_

#include "Drawing.hpp"
#include "TextAttr.hpp"

namespace draw {


/*! \brief Label Element
 * \ingroup element
 * \details This is a text label.  The object automatically
 * changes the font size so that the label will fit within the
 * specified dimensions (if possible).
 *
 */
class Text : public Drawing, public TextAttr {
public:
	/*! \brief Construct a label with text */
	Text(const char * text = 0);
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

private:

};

};

#endif /* DRAW_TEXT_HPP_ */
