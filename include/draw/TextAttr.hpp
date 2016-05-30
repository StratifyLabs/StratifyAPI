/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXTATTR_HPP_
#define DRAW_TEXTATTR_HPP_

#include "Drawing.hpp"
#include "../var/String.hpp"
#include "../var/Flags.hpp"

namespace draw {

class TextAttr {
public:
	TextAttr() { set_font_size(0); }

	/*! \details Assign a value to the string */
	void assign(const char * str){ m_str.assign(str); }

	/*! \details Get a pointer to a const char of the string */
	const char * text() const { return m_str.c_str(); }

	/*! \details Set the font size */
	inline void set_font_size(sg_size_t v){ m_font_size = v; }

	/*! \details Return the font size */
	sg_size_t font_size() const { return m_font_size; }

	/*! \details Direct access to object's String */
	var::String & str(){ return m_str; }

	/*! \details Set the font to bold (or not) */
	void set_font_bold(bool v = true){ m_flags.set_value(FLAG_BOLD, v); }

	/*! \details Returns whether font is bold or not */
	bool font_bold() const { return m_flags.value(FLAG_BOLD); }

private:
	var::String m_str;
	sg_size_t m_font_size;

	enum {
		FLAG_BOLD,
		FLAG_TOTAL
	};

	var::Flags m_flags;
};

};

#endif /* DRAW_TEXTATTR_HPP_ */
