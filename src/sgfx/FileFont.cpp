//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "sgfx/FileFont.hpp"
using namespace sgfx;
using namespace sys;

FileFont::FileFont() {
	m_kerning_pairs = 0;
}

FileFont::FileFont(const char * name, int offset) {
	m_kerning_pairs = 0;
	set_file(name, offset);
}

FileFont::~FileFont(){
	if( m_kerning_pairs != 0 ){
		free(m_kerning_pairs);
	}
	m_file.close();
}

int FileFont::set_file(const char * name, int offset){
	u32 pair_size;

	if( m_kerning_pairs ){
		free(m_kerning_pairs);
	}

	//close if not already closed
	m_file.close();

	if( m_file.open(name, File::RDONLY) < 0 ){
		return -1;
	}

	if( m_file.read(offset, &m_hdr, sizeof(m_hdr)) != sizeof(m_hdr) ){
		m_file.close();
		return -1;
	}

	m_canvas.free();
	if( m_canvas.alloc(m_hdr.canvas_width, m_hdr.canvas_height) < 0 ){
		return -1;
	}
	m_offset = offset;
	m_current_canvas = 255;
	m_canvas_start = m_hdr.size;
	m_canvas_size = m_canvas.calc_size();

	pair_size = sizeof(sg_font_kerning_pair_t)*m_hdr.kerning_pairs;

	m_kerning_pairs = (sg_font_kerning_pair_t*)malloc(pair_size);

	if( m_kerning_pairs ){
		m_file.read(m_offset + sizeof(sg_font_header_t), m_kerning_pairs, pair_size);
	}

	set_space_size(m_hdr.max_word_width);
	set_letter_spacing(m_hdr.max_height/8);

	return 0;

}

sg_size_t FileFont::get_height() const { return m_hdr.max_height; }
sg_size_t FileFont::get_width() const { return m_hdr.max_word_width*32; }


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

	offset = m_offset + sizeof(sg_font_header_t) + sizeof(sg_font_kerning_pair_t)*m_hdr.kerning_pairs + ind*sizeof(sg_font_char_t);
	if( (ret = m_file.read(offset, &ch, sizeof(ch))) != sizeof(ch) ){
		return -1;
	}
	return 0;
}

int FileFont::load_kerning(u16 first, u16 second) const {
	int kerning_count = m_hdr.kerning_pairs;
	int i;

	if( m_kerning_pairs == 0 ){
		return 0;
	}

	for(i=0; i < kerning_count; i++){
		if( (m_kerning_pairs[i].first == first) && (m_kerning_pairs[i].second == second) ){
			return m_kerning_pairs[i].kerning;
		}
	}

	return 0;
}

void FileFont::draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, sg_point_t point) const {
	u32 canvas_offset;
	if( ch.canvas_idx != m_current_canvas ){
		canvas_offset = m_canvas_start + ch.canvas_idx*m_canvas_size;
		if( m_file.read(m_offset + canvas_offset, m_canvas.data(), m_canvas_size) != (int)m_canvas_size ){
			printf("failed to load canvas %ld\n", canvas_offset);
			return;
		}
		m_current_canvas = ch.canvas_idx;
	}

	printf("Loaded canvas %d %ld (%d,%d %dx%d)\n",
			m_current_canvas,
			m_canvas_start + ch.canvas_idx*m_canvas_size,
			ch.canvas_x, ch.canvas_y, ch.width, ch.height);

	Region region(ch.canvas_x, ch.canvas_y, ch.width, ch.height);
	dest.draw_sub_bitmap(point, m_canvas, region);
}
