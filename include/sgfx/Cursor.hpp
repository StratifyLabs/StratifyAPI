/*
 * Cursor.hpp
 *
 *  Created on: Jun 8, 2017
 *      Author: tgil
 */

#ifndef SGFX_CURSOR_HPP_
#define SGFX_CURSOR_HPP_

#include "../var/Item.hpp"
#include "Bitmap.hpp"

namespace sgfx {

class Cursor : public var::Item<sg_cursor_t> {
public:
	Cursor();
	virtual ~Cursor();

	void set(const Bitmap & bitmap, sg_point_t p){ sg_api()->cursor_set(data(), bitmap.bmap_const(), p); }
	void inc_x(){ sg_api()->cursor_inc_x(data()); }
	void dec_x(){ sg_api()->cursor_dec_x(data()); }
	void inc_y(){ sg_api()->cursor_inc_y(data()); }
	void dec_y(){ sg_api()->cursor_dec_y(data()); }
	sg_color_t get_pixel(){ return sg_api()->cursor_get_pixel(data()); }
	void draw_pixel() { sg_api()->cursor_draw_pixel(data()); }
	void draw_hline(sg_size_t width){ sg_api()->cursor_draw_hline(data(), width); }
	void draw_cursor(const Cursor & src, sg_size_t width){ sg_api()->cursor_draw_cursor(data(), src.cdata(), width); }
	void shift_right(sg_size_t shift_width, sg_size_t shift_distance){ sg_api()->cursor_shift_right(data(), shift_width, shift_distance); }
	void shift_left(sg_size_t shift_width, sg_size_t shift_distance){ sg_api()->cursor_shift_left(data(), shift_width, shift_distance); }


private:
	sg_cursor_t m_cursor;
};

} /* namespace sgfx */

#endif /* SGFX_CURSOR_HPP_ */
