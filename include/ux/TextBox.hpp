/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_UX_TEXTBOX_HPP_
#define SAPI_UX_TEXTBOX_HPP_

#include "Drawing.hpp"
#include "../sgfx/Font.hpp"
#include "Text.hpp"
#include "../var/Tokenizer.hpp"

namespace ux {

/*! \brief Text Box
 * \details This class is a scrolling text box that can be used to show
 * long text messages.
 *
 */
class TextBox : public Text {
public:
	/*! \details Construct a new text box */
	TextBox();

	/*! \details Access the scroll value */
	sg_size_t scroll() const { return m_scroll; }
	/*! \details Access the max scroll value */
	sg_size_t scroll_max() const { return m_scroll_max; }

	int count_lines(sg_size_t w);

	static int count_lines(
			const sgfx::Font * font,
			const var::String & string,
			sg_size_t w
			);

	void increment_scroll(){ m_scroll++; }
	void decrement_scroll(){ if( m_scroll ){ m_scroll--; } }

	virtual void draw_to_scale(const DrawingScaledAttributes & attr);


private:
	/*! \cond */
	sg_size_t m_scroll;
	sg_size_t m_scroll_max;
	static void build_line(const sgfx::Font * font, u32 & i, var::String & line, var::Tokenizer & tokens, int & build_len, sg_size_t w);
	/*! \endcond */

};

}

#endif /* SAPI_UX_TEXTBOX_HPP_ */
