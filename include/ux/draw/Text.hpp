/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_UX_DRAW_TEXT_HPP_
#define SAPI_UX_DRAW_TEXT_HPP_

#include "../../sgfx/Font.hpp"
#include "../Drawing.hpp"

namespace ux::draw {


/*! \brief Text Class
 * \details This is a text label.  The object automatically
 * chooses the correct font height to fit within the area
 * specified.
 *
 * In order for this class to work correctly, sys::Assets::init() must
 * be invoked so that the application is aware of the system fonts.
 *
 */
class Text : public Drawing, public DrawingComponentProperties<Text> {
public:

	/*! \details Draw the scaled text as specified by \a attr */
	virtual void draw(const DrawingScaledAttributes & attr);

	Text & set_string(const var::String & value){
		m_string = value;
		return *this;
	}

	Text & set_color(sg_color_t value){
		m_color = value;
		return *this;
	}

	/*! \details Gets a pointer to the current font. */
	const sgfx::Font * font() const { return m_font; }

	/*! \details Specifies a font to use with the text.
	 *
	 * If no font is specified, the text will use sys::Assets::find_font() to
	 * load a font that most closely matches font_point_size().  If font_point_size() is
	 * zero, the text will get a font that most close matches the height
	 * of the container without exceeding the container height.
	 */
	Text & set_font(const sgfx::Font * font){	m_font = font; return *this; }

	Text & set_font_name(const var::String & value){
		m_font_name = value; return *this;
	}


	/*! \details Set the font size */
	Text & set_font_point_size(sg_size_t v){ m_font_point_size = v; return *this; }

	/*! \details Return the font size */
	sg_size_t font_point_size() const { return m_font_point_size; }

	/*! \details Sets the font style.
	 *
	 * @param style Should be a value from enum sgfx::FontInfo::style
	 *
	 *
	 */
	Text & set_font_style(enum sgfx::Font::styles style){
		m_font_style = style;
		return *this;
	}

	/*! \details Returns the font style. */
	int font_style() const { return m_font_style; }

	sg_size_t get_width(const var::String& sample, sg_size_t height);

protected:
	/*! \cond */
	const var::String & string() const { return m_string; }
	bool resolve_font(sg_size_t h);
	var::String m_string;
	var::String m_font_name;
	const sgfx::Font * m_font = nullptr;
	sg_size_t m_font_point_size = 0;
	enum sgfx::Font::styles m_font_style = sgfx::Font::style_regular;
	sg_color_t m_color;
	/*! \endcond */


};

}

#endif /* SAPI_UX_DRAW_TEXT_HPP_ */
