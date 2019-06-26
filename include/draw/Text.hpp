/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_TEXT_HPP_
#define SAPI_DRAW_TEXT_HPP_

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
class Text : public Drawing {
public:
	/*! \details Construct a label with text */
	Text(const var::ConstString & text = "");

	/*! \details Draw the scaled text as specified by \a attr */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

	Text & set_string(const var::ConstString & value){
		m_string = value;
		return *this;
	}

	Text & set_align_left(){ Drawing::set_align_left(); return *this; }
	Text & set_align_right(){ Drawing::set_align_right(); return *this; }
	Text & set_align_center(){ Drawing::set_align_center(); return *this; }
	Text & set_align_top(){ Drawing::set_align_top(); return *this; }
	Text & set_align_middle(){ Drawing::set_align_middle(); return *this; }
	Text & set_align_bottom(){ Drawing::set_align_bottom(); return *this; }

	Text & set_color(sg_color_t value){
		Drawing::set_color(value);
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
	Text & set_font(sgfx::Font * font){	m_font = font; return *this; }

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
	Text & set_font_style(u8 style){ m_font_style = style; return *this; }

	/*! \details Returns the font style. */
	int font_style() const { return m_font_style; }

protected:
	/*! \cond */
	const var::ConstString & string() const { return m_string; }
	const sgfx::Font * resolve_font(sg_size_t h) const;

private:
	var::String m_string;
	sgfx::Font * m_font;
	sg_size_t m_font_point_size;
	u8 m_font_style;
	/*! \endcond */


};

}

#endif /* SAPI_DRAW_TEXT_HPP_ */
