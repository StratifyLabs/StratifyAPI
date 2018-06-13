/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef HAL_DISPLAY_HPP_
#define HAL_DISPLAY_HPP_

#include <sos/dev/display.h>

#include "../var/Item.hpp"
#include "../var/Data.hpp"
#include "../sgfx/Bitmap.hpp"
#include "../api/HalObject.hpp"

namespace hal {

/*! \brief Display Palette Class
 * \details A display palette is used to map bitmap colors
 * to an actual display. The Stratify graphics library supports
 * the following formats.
 *
 *  - 1bpp (1 bit per pixel)
 *  - 2bpp
 *  - 4bpp
 *  - 8bpp
 *
 * This class is used to manage how the graphics pixel formats maps
 * to the display. For example, if the graphics use 4bpp and the display
 * uses RGB565, the palette would have 16 entries that contain
 * RGB565 values and define how a color in the 4bpp format maps to
 * a color on the display.
 *
 */
class DisplayPalette : public var::Item<display_palette_t>, public api::HalInfoObject {
public:

	enum {
		PIXEL_FORMAT_1BPP = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_1BPP,
		PIXEL_FORMAT_RGB565 = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_RGB565,
		PIXEL_FORMAT_RGB666 = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_RGB666,
		PIXEL_FORMAT_RGB888 = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_RGB888
	};

	int save(const char * path) const;
	int load(const char * path);

	int set_monochrome();

	void set_pixel_format(int v){ data()->pixel_format = v; }
	void set_colors(void * v, int count, int pixel_size, bool readonly = false);

	/*! \details Allocates memory for \a count colors with \a pixel_size bytes
	 * each.
	 *
	 * @param count The total number of colors
	 * @param pixel_size The number of bytes occupied by each color
	 * @return Less than zero if there were an error allocating memory
	 *
	 */
	int alloc_colors(int count, int pixel_size);

	/*! \details Sets the color.
	 *
	 * @param v The color index
	 * @param r The red component
	 * @param g The green component
	 * @param b The blue component
	 *
	 * The color format is converted based on the value of pixel_format().
	 * For example, if pixel_format() returns PIXEL_FORMAT_RGB565, the color
	 * will be set in the palette to match that format.
	 *
	 */
	void set_color(u32 v, u8 r, u8 g, u8 b);

	/*! \details Returns the pixel format. */
	u8 pixel_format() const { return item().pixel_format; }

	/*! \details Returns the number of colors in the palette. */
	u8 count() const { return item().count; }
	/*! \details Returns the number of bytes per pixel stored in the palette */
	u8 pixel_size() const { return item().pixel_size; }
	/*! \details Returns a pointer to the color index specified.
	 *
	 * @param v The color index
	 * @return A pointer to the color
	 */
	u8 * color(u32 v) const;

private:
	var::Data m_colors;
};

/*! \brief Display Class
 * \details The Display class is for drawing on a display such as an OLED or LCD.
 *
 */
class Display : public sgfx::Bitmap {
public:

	/*! \details Constructs a new object with no video memory. */
	Display(){}

	/*! \details Constructs a new object with the provided memory buffer
	 *
	 * @param mem A pointer to the bitmap memory
	 * @param w The width of the memory area in pixels
	 * @param h The height of the memory area in pixels
	 */
	Display(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h) : sgfx::Bitmap(mem,w,h){};

	/*! \details Constructs a new object and dynamically allocates memory for
	 * the buffer.
	 *
	 * @param w The width of the display
	 * @param h The height of the display
	 */
	Display(sg_size_t w, sg_size_t h) : sgfx::Bitmap(w,h){};

	/*! \details Initializes the display. */
	virtual int init(const char * name = 0) = 0;

	/*! \details Turns the display on. */
	virtual int enable() const = 0;

	/*! \details Turns the display off. */
	virtual int disable() const = 0;
};

} /* namespace hal */

#endif /* HAL_DISPLAY_HPP_ */
