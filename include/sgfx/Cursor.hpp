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
#include "../api/SgfxObject.hpp"

namespace sgfx {

class Cursor :	public api::SgfxWorkObject {
public:
	Cursor();
	virtual ~Cursor();


	void set(const Bitmap & bitmap, const Point & p){ api()->cursor_set(&m_cursor, bitmap.bmap(), p); }
	void update(const Point & p){ api()->cursor_update(&m_cursor, p); }
	void increment_x(){ api()->cursor_inc_x(&m_cursor); }
	void decrement_x(){ api()->cursor_dec_x(&m_cursor); }
	void increment_y(){ api()->cursor_inc_y(&m_cursor); }
	void decrement_y(){ api()->cursor_dec_y(&m_cursor); }
	sg_color_t get_pixel(){ return api()->cursor_get_pixel(&m_cursor); }
	sg_color_t get_pixel(int x_direction, int y_direction){
		return api()->cursor_get_pixel_increment(&m_cursor, x_direction, y_direction); }
	void draw_pixel() { api()->cursor_draw_pixel(&m_cursor); }
	void draw_hline(sg_size_t width){ api()->cursor_draw_hline(&m_cursor, width); }
	void draw_cursor(const Cursor & src, sg_size_t width){ api()->cursor_draw_cursor(&m_cursor, &src.m_cursor, width); }
	void shift_right(sg_size_t shift_width, sg_size_t shift_distance){ api()->cursor_shift_right(&m_cursor, shift_width, shift_distance); }
	void shift_left(sg_size_t shift_width, sg_size_t shift_distance){ api()->cursor_shift_left(&m_cursor, shift_width, shift_distance); }

	void inc_x(){ api()->cursor_inc_x(&m_cursor); }
	void dec_x(){ api()->cursor_dec_x(&m_cursor); }
	void inc_y(){ api()->cursor_inc_y(&m_cursor); }
	void dec_y(){ api()->cursor_dec_y(&m_cursor); }

	sg_cursor_t & cursor() { return m_cursor; }
	const sg_cursor_t & cursor() const { return m_cursor; }
	operator const sg_cursor_t & () const { return m_cursor; }

private:
	sg_cursor_t m_cursor;
};

} /* namespace sgfx */

#endif /* SGFX_CURSOR_HPP_ */
