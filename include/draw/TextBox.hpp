/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXTBOX_HPP_
#define DRAW_TEXTBOX_HPP_

#include "Drawing.hpp"
#include "TextAttr.hpp"
#include "../sgfx/Font.hpp"

namespace var {
class Token;
}

namespace draw {

/*! \brief Text Box
 * \details This class is a scrolling text box that can be used to show
 * long text messages.
 *
 */
class TextBox : public TextAttr, public Drawing {
public:
	/*! \details Construct a new text box */
	TextBox();

	/*! \details Access the scroll value */
	sg_size_t scroll() const { return m_scroll; }
	/*! \details Access the max scroll value */
	sg_size_t scroll_max() const { return m_scroll_max; }


	int count_lines(sg_size_t w);
	static int count_lines(const sgfx::Font * font, sg_size_t w, const TextAttr & text_attr);

	inline void inc_scroll(){ m_scroll++; }
	inline void dec_scroll(){ if( m_scroll ){ m_scroll--; } }

	virtual void draw_to_scale(const DrawingScaledAttr & attr);


private:


	static void build_line(const sgfx::Font * font, u32 & i, var::String & line, var::Token & tokens, int & build_len, sg_size_t w);

	sg_size_t m_scroll;
	sg_size_t m_scroll_max;
};

}

#endif /* DRAW_TEXTBOX_HPP_ */
