/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.


#ifndef SAPI_FMT_BMP_HPP_
#define SAPI_FMT_BMP_HPP_

#include <mcu/types.h>
#include "../fs/File.hpp"
#include "../api/FmtObject.hpp"
#include "../sgfx/Bitmap.hpp"

namespace fmt {

/*! \brief BMP File format */
class Bmp: public fs::File {
public:

	using Width = arg::Argument< u32, struct BmpWidthTag > ;
	using Height = arg::Argument< u32, struct BmpHeihtTag > ;
	using BitsPerPixel = arg::Argument< u16, struct BmpBitsPerPixelTag >;
	using PlaneCount = arg::Argument< u16, struct BmpPlaneCountTag >;

	/*! \details Constructs a new bitmap object and opens the bitmap as a read-only file. */
	Bmp(const var::String & name);

	/*! \details Constructs an empty bitmap object. */
	Bmp();

	static int save(
			const var::String & path,
			const sgfx::Bitmap & bitmap,
			const sgfx::Palette & pallete
			);

	sgfx::Bitmap convert_to_bitmap(
			sgfx::Bitmap::BitsPerPixel bpp
			);

	/*! \details Returns the bitmap width (after bitmap has been opened). */
	s32 width() const { return m_dib.width; }
	/*! \details Returns the bitmap height (after bitmap has been opened). */
	s32 height() const { return m_dib.height; }
	/*! \details Returns the bitmap bits per pixel (after bitmap has been opened). */
	u16 bits_per_pixel() const { return m_dib.bits_per_pixel; }
	/*! \details Returns the bitmap planes (after bitmap has been opened). */
	u16 planes() const { return m_dib.planes; }

	/*! \details Calculates the bytes needed to store one row of data (after bitmap has been opened). */
	unsigned int calculate_row_size() const;
	unsigned int calc_row_size() const { return calculate_row_size(); }

	/*! \details Opens the specified bitmap as readonly. */
	int open_readonly(const var::String & name);

	/*! \details Opens the specified bitmap as read write. */
	int open_readwrite(const var::String & name);

	/*! \details Opens the specified bitmap with the specified access (e.g., Bmp::READONLY). */
	int open(
			const var::String & name,
			const fs::OpenFlags & flags
			);

	/*! \details Creates a new bitmap using the specified parameters. */
	int create(
			const var::String & path,
			Width width,
			Height height,
			PlaneCount planes,
			BitsPerPixel bits_per_pixel
			);

	/*! \details Creates a new bitmap and save it to the /app filesystem (flash memory). */
	static int create_appfs(const var::String & name, s32 width, s32 height, u16 planes, u16 bits_per_pixel, char * img, u32 nbyte);

	/*! \details Moves file pointer to the start of the bitmap data. */
	void rewind(){ seek(Location(m_offset), SET); }

	/*! \details Seeks the file to the data at the specified row. */
	int seek_row(s32 y) const;

	/*! \details Reads a pixel from the bitmap (optionally convert to a mono value).
	 *
	 * @param pixel Data pointing to destination
	 * @param pixel_size in bytes
	 * @param mono true to convert to a mono pixel
	 * @param thres threshold brightness for a mono pixel to be on
	 */
	int read_pixel(
			u8 * pixel,
			u32 pixel_size,
			bool mono = false,
			u8 thres = 128);

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
	/*! \endcond */

private:

	bmp_dib_t m_dib;
	u32 m_offset;
};

}


#endif /* SAPI_FMT_BMP_HPP_ */
