/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "sys/Printer.hpp"
#include "sgfx/FileFont.hpp"
using namespace sgfx;
using namespace sys;

FileFont::FileFont() {
	m_kerning_pairs = 0;
}

FileFont::FileFont(
		const var::String & name,
		FileLocation offset
		) {
	m_kerning_pairs = 0;
	set_file(name, offset);
}

FileFont::~FileFont(){
	if( m_kerning_pairs != 0 ){
		free(m_kerning_pairs);
	}
}

int FileFont::set_file(
		const var::String & path,
		FileLocation offset
		){
	u32 pair_size;

	if( m_kerning_pairs ){
		free(m_kerning_pairs);
	}

	//close if not already closed
	m_file.close();

	if( m_file.open(
			 path,
			 fs::OpenFlags::read_only()) < 0 ){
		set_error_number(m_file.error_number());
		return -1;
	}

	if( m_file.read(
			 offset,
			 &m_header,
			 fs::File::Size(sizeof(m_header))
			 ) != sizeof(m_header) ){
		set_error_number(m_file.error_number());
		return -1;
	}


	m_canvas.free();
	if( m_canvas.allocate(
			 Area(m_header.canvas_width, m_header.canvas_height),
				sgfx::Bitmap::BitsPerPixel(m_header.bits_per_pixel)
			 ) < 0 ){
		set_error_number(m_canvas.error_number());
		return -1;
	}

	m_offset = offset.argument();
	m_current_canvas = 255;
	m_canvas_start = m_header.size;
	m_canvas_size = m_canvas.calculate_size();

	pair_size = sizeof(sg_font_kerning_pair_t)*m_header.kerning_pair_count;
	m_kerning_pairs = (sg_font_kerning_pair_t*)malloc(pair_size);

	if( m_kerning_pairs ){
		m_file.read(
					fs::File::Location(m_offset + sizeof(sg_font_header_t)),
					m_kerning_pairs,
					fs::File::Size(pair_size)
					);
	}

	set_space_size(m_header.max_word_width);
	set_letter_spacing(m_header.max_height/8);

	return 0;
}

sg_size_t FileFont::get_height() const { return m_header.max_height; }
sg_size_t FileFont::get_width() const { return m_header.max_word_width*32; }

int FileFont::load_char(sg_font_char_t & ch, char c, bool ascii) const {
	int offset;
	int ind;
	int ret;
	if( ascii ){
		ind = to_charset(c);
	} else {
		ind = c;
	}

	if( ind < 0 ){
		return -1;
	}

	offset = m_offset + sizeof(sg_font_header_t) + sizeof(sg_font_kerning_pair_t)*m_header.kerning_pair_count + ind*sizeof(sg_font_char_t);
	if( (ret = m_file.read(
			  fs::File::Location(offset),
			  &ch,
			  fs::File::Size(sizeof(ch))
			  ) ) != sizeof(ch) ){
		return -1;
	}

	return 0;
}

int FileFont::load_kerning(u16 first, u16 second) const {
	int kerning_count = m_header.kerning_pair_count;
	int i;

	if( m_kerning_pairs == 0 ){ return 0; }

	for(i=0; i < kerning_count; i++){
		if( (m_kerning_pairs[i].unicode_first == first) && (m_kerning_pairs[i].unicode_second == second) ){
			return m_kerning_pairs[i].horizontal_kerning;
		}
	}

	return 0;
}

sg_font_kerning_pair_t FileFont::load_kerning(u32 offset) const {
	if( offset < m_header.kerning_pair_count ){
		return m_kerning_pairs[offset];
	}
	return {0};
}

void FileFont::draw_char_on_bitmap(
		const sg_font_char_t & ch,
		Bitmap & dest,
		const Point & point
		) const {

	u32 canvas_offset;
	if( ch.canvas_idx != m_current_canvas ){
		canvas_offset = m_canvas_start + ch.canvas_idx*m_canvas_size;

		//need to read the character row by row
		if( m_file.read(
				 fs::File::Location(m_offset + canvas_offset),
				 m_canvas.to_void(),
				 fs::File::Size(m_canvas_size)
				 ) != (int)m_canvas_size ){
			return;
		}
		m_current_canvas = ch.canvas_idx;
	}

	dest.draw_sub_bitmap(
				point,
				m_canvas,
				Region(
					Point(ch.canvas_x ,ch.canvas_y),
					Area(ch.width, ch.height)
					)
				);
}
