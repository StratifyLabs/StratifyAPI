//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "sgfx/FileFont.hpp"
using namespace sgfx;
using namespace sys;

FileFont::FileFont() {
	m_kerning_pairs = 0;
}

FileFont::FileFont(const var::ConstString & name, int offset) {
	m_kerning_pairs = 0;
	set_file(name, offset);
}

FileFont::~FileFont(){
	if( m_kerning_pairs != 0 ){
		free(m_kerning_pairs);
	}
}

int FileFont::set_file(const var::ConstString & name, int offset){
	u32 pair_size;

	if( m_kerning_pairs ){
		free(m_kerning_pairs);
	}

	//close if not already closed
	m_file.close();

	if( m_file.open(name, File::RDONLY) < 0 ){
		return -1;
	}

	if( m_file.read(offset, &m_header, sizeof(m_header)) != sizeof(m_header) ){
		return -1;
	}

	m_canvas.free();
	if( m_canvas.alloc(m_header.canvas_width, m_header.canvas_height) < 0 ){
		return -1;
	}
	m_offset = offset;
	m_current_canvas = 255;
	m_canvas_start = m_header.size;
	m_canvas_size = m_canvas.calculate_size();

	pair_size = sizeof(sg_font_kerning_pair_t)*m_header.kerning_pair_count;
	m_kerning_pairs = (sg_font_kerning_pair_t*)malloc(pair_size);

	if( m_kerning_pairs ){
		m_file.read(m_offset + sizeof(sg_font_header_t), m_kerning_pairs, pair_size);
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
	if( (ret = m_file.read(offset, &ch, sizeof(ch))) != sizeof(ch) ){
		return -1;
	}

	printf("loaded character %d\n", ch.id);
	printf("loaded dims %d,%d %dx%d\n", ch.canvas_x, ch.canvas_y, ch.width, ch.height);
	printf("loaded offset %d,%d\n", ch.offset_x, ch.offset_y);
	printf("loaded advance %d\n", ch.advance_x);
	printf("loaded canvas idx %d\n", ch.canvas_idx);

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

void FileFont::draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const {
	u32 canvas_offset;
	if( ch.canvas_idx != m_current_canvas ){
		canvas_offset = m_canvas_start + ch.canvas_idx*m_canvas_size;

		//need to read the character row by row
		if( m_file.read(m_offset + canvas_offset, m_canvas.to_void(), m_canvas_size) != (int)m_canvas_size ){
			return;
		}
		m_current_canvas = ch.canvas_idx;
	}

	Region region(Point(ch.canvas_x, ch.canvas_y), Dim(ch.width, ch.height));
	dest.draw_sub_bitmap(point, m_canvas, region);
}
