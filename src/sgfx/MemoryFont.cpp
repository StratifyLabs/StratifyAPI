//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cstdio>
#include <cstring>
#include "sgfx/MemoryFont.hpp"

using namespace sgfx;


MemoryFont::MemoryFont() {
	// TODO Auto-generated constructor stub
	set_font_memory(0);
}

MemoryFont::MemoryFont(const void * ptr) {
	// TODO Auto-generated constructor stub
	set_font_memory(ptr);
}

void MemoryFont::set_font_memory(const void * ptr){
	const sg_font_header_t * hdr_ptr;
	m_font = ptr;
	if( m_font != 0 ){
		hdr_ptr = (const sg_font_header_t*)m_font;
		memcpy(&m_header, hdr_ptr, sizeof(sg_font_header_t));
		set_space_size(m_header.max_height/6);
		set_letter_spacing(m_header.max_height/8);

		m_canvas_start = m_header.size;
		m_canvas_size = m_canvas.calculate_size(Area(m_header.canvas_width, m_header.canvas_height));
	}
}

sg_size_t MemoryFont::get_height() const {
	const sg_font_header_t * hdr_ptr;
	if( m_font != 0 ){
		hdr_ptr = (const sg_font_header_t*)m_font;
		return hdr_ptr->max_height;
	}
	return 0;
}

sg_size_t MemoryFont::get_width() const {
	const sg_font_header_t * hdr_ptr;
	if( m_font != 0 ){
		hdr_ptr = (const sg_font_header_t*)m_font;
		return hdr_ptr->max_word_width*32;
	}
	return 0;
}

int MemoryFont::load_kerning(u16 first, u16 second) const {
	int kerning_count = m_header.kerning_pair_count;
	const sg_font_kerning_pair_t * pairs = (const sg_font_kerning_pair_t *)((const char *)m_font + sizeof(sg_font_header_t));
	int i;

	for(i=0; i < kerning_count; i++){
		if( (pairs[i].unicode_first == first) && (pairs[i].unicode_second == second) ){
			return pairs[i].horizontal_kerning;
		}
	}

	return 0;
}

int MemoryFont::load_char(sg_font_char_t & ch, char c, bool ascii) const {
	int offset;
	int ind;
	sg_font_char_t * chp;
	const sg_font_header_t * hdr = (const sg_font_header_t *)m_font;

	if( ascii ){
		ind = to_charset(c);
	} else {
		ind = c;
	}

	if( ind < 0 ){
		return -1;
	}

	//header has sg_font_header_t then kerning pairs then char indices
	offset = sizeof(sg_font_header_t) + sizeof(sg_font_kerning_pair_t)*hdr->kerning_pair_count + ind*sizeof(sg_font_char_t);
	chp = (sg_font_char_t *)((char*)m_font + offset);

	memcpy(&ch, chp, sizeof(sg_font_char_t));
	return 0;
}


void MemoryFont::draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const {
	u32 canvas_offset = m_canvas_start + m_canvas_size * ch.canvas_idx;
	m_canvas.set_data((sg_bmap_data_t*)((u8*)m_font + canvas_offset), m_header.canvas_width, m_header.canvas_height, true);
	Region region(Point(ch.canvas_x, ch.canvas_y), Area(ch.width, ch.height));
	dest.draw_sub_bitmap(point, m_canvas, region);
}


