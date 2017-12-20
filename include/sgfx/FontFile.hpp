/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONTFILE_HPP_
#define SGFX_FONTFILE_HPP_

#include "Font.hpp"
#include "../sys/File.hpp"

namespace sgfx {

class FontFile : public Font {
public:
	FontFile();
	FontFile(const char * name, int offset = 0);
	~FontFile();

	int set_file(const char * name, int offset = 0);

	sg_size_t get_height() const;
	sg_size_t get_width() const;
	const Bitmap & bitmap() const;

protected:
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	int load_bitmap(const sg_font_char_t & ch) const;
	int load_kerning(u16 first, u16 second) const;

private:
	mutable sys::File m_file;
	mutable Bitmap m_bitmap;
	sg_font_kerning_pair_t * m_kerning_pairs;

};

};

#endif /* SGFX_FONTFILE_HPP_ */
