//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Draw/MBFontFile.hpp"
using namespace Draw;
using namespace Sys;

MBFontFile::MBFontFile() {
	// TODO Auto-generated constructor stub
	_bitmap = 0;
	pairs = 0;
}

MBFontFile::MBFontFile(const char * name, int offset) {
	// TODO Auto-generated constructor stub
	_bitmap = 0;
	pairs = 0;
	set_file(name, offset);
}

MBFontFile::~MBFontFile(){
	if( _bitmap != 0 ){
		delete _bitmap;
	}
	if( pairs != 0 ){
		free(pairs);
	}
	f.close();
}

int MBFontFile::set_file(const char * name, int offset){
	size_t pair_size;

	if( _bitmap != 0 ){
		delete _bitmap;
	}

	if( pairs ){
		free(pairs);
	}

	//close if not already closed
	f.close();

	if( f.open(name, File::RDONLY) < 0 ){
		return -1;
	}

	if( f.read(offset, &hdr, sizeof(hdr)) != sizeof(hdr) ){
		f.close();
		return -1;
	}

	_bitmap = new MBitmap(hdr.max_byte_width*8, hdr.max_height);
	_offset = offset;

	pair_size = sizeof(mbfont_kerning_pair_t)*hdr.kerning_pairs;

	pairs = (mbfont_kerning_pair_t*)malloc(pair_size);

	if( pairs ){
		f.read(_offset + sizeof(mbfont_hdr_t), pairs, pair_size);
	}

	set_space_size(hdr.max_byte_width/4);
	set_letter_spacing(hdr.max_height/8);

	return 0;

}

const MBitmap * MBFontFile::bitmap(char c, bool ascii) const {
	if( _bitmap == 0 ){
		return 0;
	}

	//load character header info
	if( load_char(_ch, c, ascii) < 0 ){
		return 0;
	}

	//load bitmap
	if ( load_bitmap(_ch) < 0 ){
		return 0;
	}

	return _bitmap;
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
	offset = _offset + sizeof(mbfont_hdr_t) + sizeof(mbfont_kerning_pair_t)*hdr.kerning_pairs + ind*sizeof(mbfont_char_t);
	if( (ret = f.read(offset, &ch, sizeof(ch))) != sizeof(ch) ){
		return -1;
	}
	return 0;
}

int MBFontFile::load_kerning(u16 first, u16 second) const {
	int kerning_count = hdr.kerning_pairs;
	int i;

	if( pairs == 0 ){
		return 0;
	}

	for(i=0; i < kerning_count; i++){

		if( (pairs[i].first == first) && (pairs[i].second == second) ){
			return pairs[i].kerning;
		}
	}

	return 0;
}

int MBFontFile::load_bitmap(const mbfont_char_t & ch) const {
	int s;
	//calculate number of bytes to read
	s = MBitmap::size(ch.width, ch.height);
	if( f.read(ch.offset + _offset, _bitmap->data(), s) != s ){
		return -1;
	}
	_bitmap->setsize(ch.width, ch.height);
	return 0;
}
