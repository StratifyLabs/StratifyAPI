/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_SGFX_CURSOR_HPP_
#define SAPI_SGFX_CURSOR_HPP_

#include "../var/View.hpp"
#include "Bitmap.hpp"
#include "../api/SgfxObject.hpp"

namespace sgfx {

class Cursor :	public api::SgfxWorkObject {
public:
	Cursor();
	Cursor(const Bitmap & bitmap, const Point & p);


	Cursor & set_bitmap(const Bitmap & bitmap){ api()->cursor_set(&m_cursor, bitmap.bmap(), sg_point(0,0)); return *this; }
	Cursor & set_point(const Point & p){ api()->cursor_update(&m_cursor, p); return *this; }
	Cursor & operator = (const Bitmap & bitmap){ return set_bitmap(bitmap); }
	Cursor & operator = (const Point & p){ return set_point(p); }
	Cursor & increment_x(){ api()->cursor_inc_x(&m_cursor); return *this; }
	Cursor & decrement_x(){ api()->cursor_dec_x(&m_cursor); return *this; }
	Cursor & increment_y(){ api()->cursor_inc_y(&m_cursor); return *this; }
	Cursor & decrement_y(){ api()->cursor_dec_y(&m_cursor); return *this; }
	sg_color_t get_pixel(){ return api()->cursor_get_pixel(&m_cursor); }
	sg_color_t get_pixel(int x_direction, int y_direction){
		return api()->cursor_get_pixel_increment(&m_cursor, x_direction, y_direction); }
	void draw_pixel() { api()->cursor_draw_pixel(&m_cursor); }
	void draw_hline(sg_size_t width){ api()->cursor_draw_hline(&m_cursor, width); }
	void draw_cursor(const Cursor & src, sg_size_t width){ api()->cursor_draw_cursor(&m_cursor, &src.m_cursor, width); }
	void shift_right(sg_size_t shift_width, sg_size_t shift_distance){ api()->cursor_shift_right(&m_cursor, shift_width, shift_distance); }
	void shift_left(sg_size_t shift_width, sg_size_t shift_distance){ api()->cursor_shift_left(&m_cursor, shift_width, shift_distance); }
	sg_size_t find_positive_edge(sg_size_t width){ return api()->cursor_find_positive_edge(&m_cursor, width); }
	sg_size_t find_negative_edge(sg_size_t width){ return api()->cursor_find_negative_edge(&m_cursor, width); }

	Cursor & inc_x(){	api()->cursor_inc_x(&m_cursor); return *this; }
	Cursor & dec_x(){	api()->cursor_dec_x(&m_cursor); return *this; }
	Cursor & inc_y(){	api()->cursor_inc_y(&m_cursor); return *this; }
	Cursor & dec_y(){ api()->cursor_dec_y(&m_cursor); return *this; }

	sg_cursor_t & cursor() { return m_cursor; }
	const sg_cursor_t & cursor() const { return m_cursor; }
	operator const sg_cursor_t & () const { return m_cursor; }

private:
	sg_cursor_t m_cursor;
};

class EdgeDetector {
public:
	EdgeDetector(
			const Bitmap & bitmap,
			const Region & region = Region()
			){
		set_bitmap(bitmap);
		set_region(region.is_valid() ? region : bitmap.region());
	}

	EdgeDetector & set_bitmap(const Bitmap & bitmap){
		m_cursor.set_bitmap(bitmap);
		return reset();
	}

	EdgeDetector & set_region(const Region & region){
		m_region = region;
		return reset();
	}

	Point find_next();

	EdgeDetector & reset(){
		m_cursor_point = Point();
		m_is_negative = false;
		m_cursor.set_point(m_region.point());
		return *this;
	}


	void draw_edges(Bitmap & bitmap, const Point & start_point = Point());

	bool is_next_positive() const { return !m_is_negative; }
	bool is_next_negative() const { return m_is_negative; }


	const Region & region(){ return m_region; }
	const Point & point(){ return m_cursor_point; }

private:
	bool m_is_negative;
	Point m_cursor_point;
	Region m_region;
	Cursor m_cursor;

};

} /* namespace sgfx */

namespace sys {
class Printer;
Printer & operator << (Printer& printer, const sgfx::Cursor & a);
}

#endif /* SAPI_SGFX_CURSOR_HPP_ */
