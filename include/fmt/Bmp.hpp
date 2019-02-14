/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef BMP_HPP_
#define BMP_HPP_

#include <mcu/types.h>
#include "../sys/File.hpp"
#include "../api/FmtObject.hpp"

namespace fmt {

/*! \brief BMP File format */
class Bmp: public api::FmtFileObject {
public:

	/*! \details Constructs a new bitmap object and opens the bitmap as a read-only file. */
	Bmp(const var::ConstString & name);

	/*! \details Constructs an empty bitmap object. */
	Bmp();

	/*! \details Returns the bitmap width (after bitmap has been opened). */
	s32 width() const { return m_dib.width; }
	/*! \details Returns the bitmap height (after bitmap has been opened). */
	s32 height() const { return m_dib.height; }
	/*! \details Returns the bitmap bits per pixel (after bitmap has been opened). */
	u16 bits_per_pixel() const { return m_dib.bits_per_pixel; }
	/*! \details Returns the bitmap planes (after bitmap has been opened). */
	u16 planes() const { return m_dib.planes; }

	/*! \details Calculates the bytes needed to store one row of data (after bitmap has been opened). */
	unsigned int calc_row_size() const;

	/*! \details Opens the specified bitmap as readonly. */
	int open_readonly(const var::ConstString & name);

	/*! \details Opens the specified bitmap as read write. */
	int open_readwrite(const var::ConstString & name);

	/*! \details Opens the specified bitmap with the specified access (e.g., Bmp::READONLY). */
	int open(const var::ConstString & name, int access);

	/*! \details Creates a new bitmap using the specified parameters. */
	int create(const var::ConstString & name, s32 width, s32 height, u16 planes, u16 bits_per_pixel);

	/*! \details Creates a new bitmap and save it to the /app filesystem (flash memory). */
	static int create_appfs(const var::ConstString & name, s32 width, s32 height, u16 planes, u16 bits_per_pixel, char * img, u32 nbyte);

	/*! \details Moves file pointer to the start of the bitmap data. */
	void rewind(){ seek(m_offset); }

	/*! \details Seeks the file to the data at the specified row. */
	int seek_row(s32 y) const;

	/*! \details Reads a pixel from the bitmap (optionally convert to a mono value).
	 *
	 * @param pixel Data pointing to destination
	 * @param pixel_size in bytes
	 * @param mono true to convert to a mono pixel
	 * @param thres threshold brightness for a mono pixel to be on
	 */
	int read_pixel(u8 * pixel, u32 pixel_size, bool mono = false, u8 thres = 128);

	/*! \cond */
	typedef struct MCU_PACK {
		u16 signature;
		u32 size;
		u16 resd1;
		u16 resd2;
		u32 offset;
	} bmp_header_t;

	typedef struct MCU_PACK {
		u32 hdr_size;
		s32 width;
		s32 height;
		u16 planes;
		u16 bits_per_pixel;
	} bmp_dib_t;

	enum {
		SIGNATURE = 0x4D42
	};
	/*! \bmpcond */

private:

	bmp_dib_t m_dib;
	u32 m_offset;
};

}


#endif /* BMP_HPP_ */
