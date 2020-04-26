/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef DRAW_TEXTATTR_HPP_
#define DRAW_TEXTATTR_HPP_

#include "Drawing.hpp"
#include "../var/String.hpp"
#include "../var/Flags.hpp"
#include "../sgfx/Font.hpp"

/*! \cond */
namespace draw {

/*! \brief Text Attribute Class
 * \details This class contains the
 * information needed to draw text.  It allows
 * the text to be stored separate from the drawing object.
 */
class TextAttributes : public api::InfoObject {
public:
	TextAttributes() {
		set_font_size(0);
		m_font = 0;
		set_font_style(sgfx::FontInfo::style_regular);
	}

	/*! \details Gets a reference to the text string. */
	var::String & string(){ return m_string; }

	/*! \details Gets a reference to the text string (read-only). */
	const var::String & string() const { return m_string; }

	/*! \details Gets a pointer to the current font. */
	const sgfx::Font * font() const { return m_font; }

	/*! \details Specify a font
	 *
	 * If no font is specified, the text will use sys::Assets::find_font() to
	 * load a font that most closely matches font_size().  If font_size() is
	 * zero, the text will get a font that most close matches the height
	 * of the container without exceeding the container height.
	 */
	sgfx::Font * set_font(sgfx::Font * font){ return m_font = font; }

	/*! \details Set the font size */
	void set_font_size(sg_size_t v){ m_font_size = v; }

	/*! \details Return the font size */
	sg_size_t font_size() const { return m_font_size; }

	/*! \details Sets the font style.
	 *
	 * @param style Should be a value from enum sgfx::FontInfo::style
	 *
	 *
	 */
	void set_font_style(u8 style){ m_font_style = style; }

	/*! \details Returns whether font is bold or not */
	bool font_style() const { return m_font_style; }

protected:
	const sgfx::Font * resolve_font(sg_size_t h) const;

private:
	var::String m_string;
	sgfx::Font * m_font;

	sg_size_t m_font_size;
	u8 m_font_style;
};

typedef TextAttributes TextAttr;

}

/*! \endcond */

#endif /* DRAW_TEXTATTR_HPP_ */
