//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Draw/MBFontFile.hpp"
using namespace Draw;
using namespace Sys;

MBFontFile::MBFontFile() {
	// TODO Auto-generated constructor stub
	m_bitmap = 0;
	m_kerning_pairs = 0;
}

MBFontFile::MBFontFile(const char * name, int offset) {
	// TODO Auto-generated constructor stub
	m_bitmap = 0;
	m_kerning_pairs = 0;
	set_file(name, offset);
}

MBFontFile::~MBFontFile(){
	if( m_bitmap != 0 ){
		delete m_bitmap;
	}
	if( m_kerning_pairs != 0 ){
		free(m_kerning_pairs);
	}
	m_file.close();
}

int MBFontFile::set_file(const char * name, int offset){
	size_t pair_size;

	if( m_bitmap != 0 ){
		delete m_bitmap;
	}

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

	m_bitmap = new MBitmap(m_hdr.max_byte_width*8, m_hdr.max_height);
	m_offset = offset;

	pair_size = sizeof(mbfont_kerning_pair_t)*m_hdr.kerning_pairs;

	m_kerning_pairs = (mbfont_kerning_pair_t*)malloc(pair_size);

	if( m_kerning_pairs ){
		m_file.read(m_offset + sizeof(mbfont_hdr_t), m_kerning_pairs, pair_size);
	}

	set_space_size(m_hdr.max_byte_width/4);
	set_letter_spacing(m_hdr.max_height/8);

	return 0;

}

const MBitmap * MBFontFile::bitmap(char c, bool ascii) const {
	if( m_bitmap == 0 ){
		return 0;
	}

	//load character header info
	if( load_char(m_char, c, ascii) < 0 ){
		return 0;
	}

	//load bitmap
	if ( load_bitmap(m_char) < 0 ){
		return 0;
	}

	return m_bitmap;
}

int MBFontFile::load_char(mbfont_char_t & ch, char c, bool ascii) const {
	int offset;
	int ind;
	int ret;
	if( ascii ){
		ind = to_charset(c);
	} else {
		ind = c;
	}
	offset = m_offset + sizeof(mbfont_hdr_t) + sizeof(mbfont_kerning_pair_t)*m_hdr.kerning_pairs + ind*sizeof(mbfont_char_t);
	if( (ret = m_file.read(offset, &ch, sizeof(ch))) != sizeof(ch) ){
		return -1;
	}
	return 0;
}

int MBFontFile::load_kerning(u16 first, u16 second) const {
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

int MBFontFile::load_bitmap(const mbfont_char_t & ch) const {
	int s;
	//calculate number of bytes to read
	s = MBitmap::calc_size(ch.width, ch.height);
	if( m_file.read(ch.offset + m_offset, m_bitmap->data(), s) != s ){
		return -1;
	}
	m_bitmap->set_size(ch.width, ch.height);
	return 0;
}
