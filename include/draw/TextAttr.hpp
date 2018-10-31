/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXTATTR_HPP_
#define DRAW_TEXTATTR_HPP_

#include "Drawing.hpp"
#include "../var/String.hpp"
#include "../var/Flags.hpp"
#include "../sgfx/Font.hpp"

namespace draw {

/*! \brief Text Attribute Class
 * \details This class contains the
 * information needed to draw text.  It allows
 * the text to be stored separate from the drawing object.
 */
class TextAttr : public api::DrawInfoObject {
public:
	TextAttr() { set_font_size(0); m_font = 0; }

	/*! \details Assign a value to the string */
	void assign(const var::ConstString & str){ m_str.assign(str); }

	/*! \details Get a pointer to a const char of the string */
	const char * text() const { return m_str.c_str(); }

	/*! \details Get a pointer to the specified font */
	const sgfx::Font * font() const { return m_font; }

	/*! \details Specify a font
	 *
	 * If no font is specified, the text will use sys::Assets::get_font() to
	 * load a font that most closely matches font_size().  If font_size() is
	 * zero, the text will get a font that most close matches the height
	 * of the container without exceeding the container height.
	 */
	sgfx::Font * set_font(sgfx::Font * font){ return m_font = font; }

	/*! \details Set the font size */
	void set_font_size(sg_size_t v){ m_font_size = v; }

	/*! \details Return the font size */
	sg_size_t font_size() const { return m_font_size; }

	/*! \details Direct access to object's String */
	var::String & str(){ return m_str; }

	/*! \details Set the font to bold (or not) */
	void set_font_bold(bool v = true){ m_flags.set_value(FLAG_BOLD, v); }

	/*! \details Returns whether font is bold or not */
	bool font_bold() const { return m_flags.value(FLAG_BOLD); }

protected:
	const sgfx::Font * resolve_font(sg_size_t h) const;

private:
	var::String m_str;
	sg_size_t m_font_size;
	sgfx::Font * m_font;

	enum {
		FLAG_BOLD,
		FLAG_TOTAL
	};

	var::Flags m_flags;
};

}

#endif /* DRAW_TEXTATTR_HPP_ */
