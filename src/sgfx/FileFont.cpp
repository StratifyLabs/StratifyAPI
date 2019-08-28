//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "sgfx/FileFont.hpp"
using namespace sgfx;
using namespace sys;

FileFont::FileFont() {
	m_kerning_pairs = 0;
}

FileFont::FileFont(const arg::SourceFilePath & name, const arg::Location & offset) {
	m_kerning_pairs = 0;
	set_file(name, offset);
}

FileFont::~FileFont(){
	if( m_kerning_pairs != 0 ){
		free(m_kerning_pairs);
	}
}

int FileFont::set_file(const arg::SourceFilePath & name, const arg::Location & offset){
	u32 pair_size;

	if( m_kerning_pairs ){
		free(m_kerning_pairs);
	}

	//close if not already closed
	m_file.close();

	if( m_file.open(
			 arg::FilePath(name.argument()),
			 fs::OpenFlags::read_only()) < 0 ){
		set_error_number(m_file.error_number());
		return -1;
	}

	if( m_file.read(
			 offset,
			 arg::DestinationBuffer(&m_header),
			 arg::Size(sizeof(m_header))
			 ) != sizeof(m_header) ){
		set_error_number(m_file.error_number());
		return -1;
	}


	m_canvas.free();
	m_canvas.set_bits_per_pixel(m_header.bits_per_pixel);
	if( m_canvas.allocate(
			 Area(
				 arg::Width(m_header.canvas_width),
				 arg::Height(m_header.canvas_height)
				 )
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
					arg::Location(m_offset + sizeof(sg_font_header_t)),
					arg::DestinationBuffer(m_kerning_pairs),
					arg::Size(pair_size)
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
			  arg::Location(offset),
			  arg::DestinationBuffer(&ch),
			  arg::Size(sizeof(ch))
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

void FileFont::draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const {
	u32 canvas_offset;
	if( ch.canvas_idx != m_current_canvas ){
		canvas_offset = m_canvas_start + ch.canvas_idx*m_canvas_size;

		//need to read the character row by row
		if( m_file.read(
				 arg::Location(m_offset + canvas_offset),
				 arg::DestinationBuffer(m_canvas.to_void()),
				 arg::Size(m_canvas_size)
				 ) != (int)m_canvas_size ){
			return;
		}
		m_current_canvas = ch.canvas_idx;
	}

	Region region(
				Point(
					arg::XValue(ch.canvas_x),
					arg::YValue(ch.canvas_y)
					),
				Area(
					arg::Width(ch.width),
					arg::Height(ch.height)
					)
				);
	dest.draw_sub_bitmap(point, m_canvas, region);
}
