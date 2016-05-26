/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MBFONTFILE_HPP_
#define MBFONTFILE_HPP_

#include <draw/MBFont.hpp>
#include <sys/File.hpp>

namespace draw {

class MBFontFile : public MBFont {
public:
	MBFontFile();
	MBFontFile(const char * name, int offset = 0);
	~MBFontFile();

	int set_file(const char * name, int offset = 0);

	const MBitmap * bitmap(char c, bool ascii = true) const;

protected:
	int load_char(mbfont_char_t & ch, char c, bool ascii) const;
	int load_bitmap(const mbfont_char_t & ch) const;
	int load_kerning(u16 first, u16 second) const;



private:
	mutable sys::File m_file;
	mbfont_kerning_pair_t * m_kerning_pairs;

};

};

#endif /* MBFONTFILE_HPP_ */
