/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef BMP_HPP_
#define BMP_HPP_

#include <mcu/types.h>
#include <sys/File.hpp>

namespace fmt {

/*! \brief BMP File format */
class Bmp: public sys::File {
public:
	Bmp(const char * name);
	Bmp();

	i32 w() const { return m_dib.width; }
	i32 h() const { return m_dib.height; }
	u16 bits_per_pixel() const { return m_dib.bits_per_pixel; }
	u16 planes() const { return m_dib.planes; }


	unsigned int calc_row_size() const;

	int create(const char * name, i32 width, i32 height, u16 planes, u16 bits_per_pixel);
	static int create_appfs(const char * name, i32 width, i32 height, u16 planes, u16 bits_per_pixel, char * img, size_t nbyte);

	void rewind(){ seek(m_offset); }

	int seek_row(i32 y);

	int read_pixel(u8 * pixel, ssize_t pixel_size, bool mono = false, u8 thres = 128);

	typedef struct MCU_PACK {
		u16 signature;
		u32 size;
		u16 resd1;
		u16 resd2;
		u32 offset;
	} bmp_header_t;

	typedef struct MCU_PACK {
		u32 hdr_size;
		i32 width;
		i32 height;
		u16 planes;
		u16 bits_per_pixel;
	} bmp_dib_t;

	enum {
		SIGNATURE = 0x424D
	};

private:

	bmp_dib_t m_dib;
	u32 m_offset;
};

};


#endif /* BMP_HPP_ */
