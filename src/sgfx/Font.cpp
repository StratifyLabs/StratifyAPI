/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <errno.h>
#include "var/Token.hpp"
#include "fs/File.hpp"
#include "sgfx/Font.hpp"

using namespace sgfx;

FontInfo::FontInfo(
		PointSize point_size,
		Style style,
		Font * font){
	m_point_size = point_size.argument();
	m_style = style.argument();
	m_font = font;
}

FontInfo::FontInfo(const var::String & path){
	m_path = path;

	var::Tokenizer tokens(
				fs::File::name(path),
				var::Tokenizer::Delimeters("-.")
				);

	if( tokens.count() != 4 ){
		m_point_size = 0;
	} else {

		m_font = 0;
		m_name = tokens.at(0);
		m_point_size = var::String(tokens.at(2)).to_integer();
		var::String style = tokens.at(1);
		style.to_lower();

		m_style = ANY;
		if( style == "t" ){ m_style = THIN; }
		if( style == "ti" ){ m_style = THIN_ITALIC; }
		if( style == "el" ){ m_style = EXTRA_LIGHT; }
		if( style == "eli" ){ m_style = EXTRA_LIGHT_ITALIC; }
		if( style == "l" ){ m_style = LIGHT; }
		if( style == "li" ){ m_style = LIGHT_ITALIC; }
		if( style == "r" ){ m_style = REGULAR; }
		if( style == "ri" ){ m_style = REGULAR_ITALIC; }
		if( style == "m" ){ m_style = MEDIUM; }
		if( style == "mi" ){ m_style = MEDIUM_ITALIC; }
		if( style == "sb" ){ m_style = SEMI_BOLD; }
		if( style == "sbi" ){ m_style = SEMI_BOLD_ITALIC; }
		if( style == "b" ){ m_style = BOLD; }
		if( style == "bi" ){ m_style = BOLD_ITALIC; }
		if( style == "eb" ){ m_style = EXTRA_BOLD; }
		if( style == "ebi" ){ m_style = EXTRA_BOLD_ITALIC; }
		if( style == "ico" ){ m_style = ICONS; }
	}
}


bool FontInfo::ascending_point_size(
		const FontInfo & a,
		const FontInfo & b
		){
	return a.point_size() < b.point_size();
}

bool FontInfo::ascending_style(
		const FontInfo & a,
		const FontInfo & b
		){
	return a.style() < b.style();
}

const var::String Font::m_ascii_character_set = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

const var::String & Font::ascii_character_set(){
	return m_ascii_character_set;
}

int Font::to_charset(char ascii){
	if( (ascii < ' ') || (ascii > '~') ){
		return -1;
	}
	return (int)(ascii - ' ' - 1);
}

Font::Font() {
	//m_bitmap = 0;
	m_offset = 0;
	m_space_size = 8;
	m_letter_spacing = 1;
	m_is_kerning_enabled = true;
}

int Font::calculate_length(const var::String & str) const {
	u32 length = 0;
	const char * s = str.cstring();
	while( *s != 0 ){

		if( *s == ' ' ){
			length += space_size();
		} else {
			if( load_char(m_char, *s, true) == 0){
				length += m_char.advance_x;
			}
		}
		s++;
	}
	return length;
}

int Font::draw(char c, Bitmap & dest, const Point & point) const {

	if( load_char(m_char, c, true) < 0 ){
		return -1;
	}

	Point p = point + Point(m_char.offset_x, m_char.offset_y);

	draw_char_on_bitmap(m_char, dest, p);

	return m_char.advance_x;
}


sg_font_char_t Font::character(u32 offset){
	sg_font_char_t result = {0};
	load_char(result, offset, false);
	return result;
}

Bitmap Font::character_bitmap(u32 offset){
	sg_font_char_t character = this->character(offset);

	Bitmap result(
				Area(
					character.width,
					character.height
					),
				Bitmap::BitsPerPixel(bits_per_pixel())
				);

	draw_char_on_bitmap(character, result, Point(0,0));
	return result;
}

int Font::draw(
		const var::String & const_string,
		Bitmap & bitmap,
		const Point & point
		) const {
	sg_size_t w;

	//draw characters on the bitmap
	Point p(point); //copy that can be advanced

	for(u32 i=0; i < const_string.length(); i++){
		char c = const_string.at(i);
		char d;
		if( i < const_string.length() - 1 ){
			d = const_string.at(i+1);
		} else {
			d = 0;
		}
		if( c == ' ' ){
			w = space_size();
		} else {
			draw(c, bitmap, p);
			w = m_char.advance_x;
		}

		//apply kerning
		if( is_kerning_enabled() && d != 0 ){
			w -= load_kerning(c, d);
			if( w < 0 ){
				return -1;
			}
		}

		p += Point::X(w);

	}

	return 0;
}

