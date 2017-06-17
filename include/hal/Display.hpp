/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef HAL_DISPLAY_HPP_
#define HAL_DISPLAY_HPP_

#include <iface/dev/display.h>

#include "../var/Item.hpp"
#include "../var/Data.hpp"
#include "../sgfx/Bitmap.hpp"

namespace hal {

class DisplayPalette : public var::Item<display_palette_t> {
public:

	enum {
		PIXEL_FORMAT_1BPP = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_1BPP,
		PIXEL_FORMAT_RGB565 = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_RGB565,
		PIXEL_FORMAT_RGB666 = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_RGB666,
		PIXEL_FORMAT_RGB888 = DISPLAY_PALETTE_ATTR_PIXEL_FORMAT_RGB888
	};

	int save(const char * path) const;
	int load(const char * path);

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

	void set_color(size_t v, u8 r, u8 g, u8 b);

	u8 pixel_format() const { return item().pixel_format; }
	u8 count() const { return item().count; }
	u8 pixel_size() const { return item().pixel_size; }
	u8 * color(size_t v) const;

private:

	var::Data m_colors;


};

class Display : public sgfx::Bitmap {
public:
	Display(){}

	Display(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h);

	/*! \details Pure virtual function to initialize the LCD */
	virtual int init(const char * name = 0) = 0;

	/*! \details Turn the LCD pixels on */
	virtual int on(){ return -1; };

	/*! \details Turn the LCD pixels off */
	virtual int off(){ return -1; };
};

} /* namespace hal */

#endif /* HAL_DISPLAY_HPP_ */
