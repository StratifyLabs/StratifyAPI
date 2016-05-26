//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Draw/MBFontMemory.hpp"
using namespace Draw;



MBFontMemory::MBFontMemory() {
	// TODO Auto-generated constructor stub
	set_font_memory(0);
}

MBFontMemory::MBFontMemory(const void * ptr) {
	// TODO Auto-generated constructor stub
	set_font_memory(ptr);
}

void MBFontMemory::set_font_memory(const void * ptr){
	const mbfont_hdr_t * hdr_ptr;
	m_font = ptr;
	if( m_font != 0 ){
		hdr_ptr = (const mbfont_hdr_t*)m_font;
		memcpy(&m_hdr, hdr_ptr, sizeof(mbfont_hdr_t));
		set_space_size(m_hdr.max_byte_width/4);
		set_letter_spacing(m_hdr.max_height/8);
	}
}


const MBitmap * MBFontMemory::bitmap(char c, bool ascii) const {

	//load character header info
	if( load_char(m_char, c, ascii) < 0 ){
		return 0;
	}

	//load bitmap
	if ( load_bitmap(m_char) < 0 ){
		return 0;
	}

	return &m_bitmap;
}

int MBFontMemory::load_kerning(u16 first, u16 second) const {
	int kerning_count = m_hdr.kerning_pairs;
	const mbfont_kerning_pair_t * pairs = (const mbfont_kerning_pair_t *)((const char *)m_font + sizeof(mbfont_hdr_t));
	int i;

	for(i=0; i < kerning_count; i++){
		if( (pairs[i].first == first) && (pairs[i].second == second) ){
			return pairs[i].kerning;
		}
	}

	return 0;
}

int MBFontMemory::load_char(mbfont_char_t & ch, char c, bool ascii) const {
	int offset;
	int ind;
	mbfont_char_t * chp;
	const mbfont_hdr_t * hdr = (const mbfont_hdr_t *)m_font;

	if( ascii ){
		ind = to_charset(c);
	} else {
		ind = c;
	}



	//header has mbfont_hdr_t then kerning pairs then char indices
	offset = sizeof(mbfont_hdr_t) + sizeof(mbfont_kerning_pair_t)*hdr->kerning_pairs + ind*sizeof(mbfont_char_t);
	chp = (mbfont_char_t *)((char*)m_font + offset);

	memcpy(&ch, chp, sizeof(mbfont_char_t));

	return 0;
}

int MBFontMemory::load_bitmap(const mbfont_char_t & ch) const {
	m_bitmap.set_data((uint8_t*)m_font + ch.offset, ch.width, ch.height, true);
	return 0;
}


