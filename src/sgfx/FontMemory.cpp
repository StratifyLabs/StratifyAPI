//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cstring>
#include "sgfx/FontMemory.hpp"

using namespace sgfx;


FontMemory::FontMemory() {
	// TODO Auto-generated constructor stub
	set_font_memory(0);
}

FontMemory::FontMemory(const void * ptr) {
	// TODO Auto-generated constructor stub
	set_font_memory(ptr);
}

void FontMemory::set_font_memory(const void * ptr){
	const sg_font_header_t * hdr_ptr;
	m_font = ptr;
	if( m_font != 0 ){
		hdr_ptr = (const sg_font_header_t*)m_font;
		memcpy(&m_hdr, hdr_ptr, sizeof(sg_font_header_t));
		set_space_size(m_hdr.max_byte_width/4);
		set_letter_spacing(m_hdr.max_height/8);
	}
}

u16 FontMemory::get_h() const {
	const sg_font_header_t * hdr_ptr;
	if( m_font != 0 ){
		hdr_ptr = (const sg_font_header_t*)m_font;
		return hdr_ptr->max_height;
	}
	return 0;
}




const Bitmap & FontMemory::bitmap() const {

	//load character header info
	//load_char(m_char, c, ascii);
	//load bitmap
	load_bitmap(m_char);
	return m_bitmap;
}

int FontMemory::load_kerning(u16 first, u16 second) const {
	int kerning_count = m_hdr.kerning_pairs;
	const sg_font_kerning_pair_t * pairs = (const sg_font_kerning_pair_t *)((const char *)m_font + sizeof(sg_font_header_t));
	int i;

	for(i=0; i < kerning_count; i++){
		if( (pairs[i].first == first) && (pairs[i].second == second) ){
			return pairs[i].kerning;
		}
	}

	return 0;
}

int FontMemory::load_char(sg_font_char_t & ch, char c, bool ascii) const {
	int offset;
	int ind;
	sg_font_char_t * chp;
	const sg_font_header_t * hdr = (const sg_font_header_t *)m_font;

	if( ascii ){
		ind = to_charset(c);
	} else {
		ind = c;
	}



	//header has sg_font_header_t then kerning pairs then char indices
	offset = sizeof(sg_font_header_t) + sizeof(sg_font_kerning_pair_t)*hdr->kerning_pairs + ind*sizeof(sg_font_char_t);
	chp = (sg_font_char_t *)((char*)m_font + offset);

	memcpy(&ch, chp, sizeof(sg_font_char_t));

	return 0;
}

int FontMemory::load_bitmap(const sg_font_char_t & ch) const {
	m_bitmap.set_data((sg_bmap_data_t*)m_font + ch.offset, ch.width, ch.height, true);
	return 0;
}


