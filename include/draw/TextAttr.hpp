/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXTATTR_HPP_
#define DRAW_TEXTATTR_HPP_

#include "Drawing.hpp"
#include "../var/String.hpp"

namespace draw {

class TextAttr {
public:
	TextAttr() { set_font_size(0); }

	/*! \brief Assign a value to the string */
	void assign(const char * str){ m_str.assign(str); }

	/*! \brief Get a pointer to a const char of the string */
	const char * text() const { return m_str.c_str(); }

	/*! \brief Set the font size */
	inline void set_font_size(sg_size_t v){ m_font_size = v; }

	/*! \brief Return the font size */
	sg_size_t font_size() const { return m_font_size; }

	/*! \brief Direct access to object's String */
	var::String & str(){ return m_str; }

	//void set_font_bold(bool v = true ){ m_font_bold = v; }
	//bool font_bold() const { return m_font_bold; }

private:
	var::String m_str;
	sg_size_t m_font_size;
	//bool m_font_bold;
};

};

#endif /* DRAW_TEXTATTR_HPP_ */
