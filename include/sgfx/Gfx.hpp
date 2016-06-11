/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_GFX_HPP_
#define SGFX_GFX_HPP_

#include <sgfx/sg_types.h>

#include "Bitmap.hpp"
#include "../var/Item.hpp"

namespace sgfx {

/*! \brief Gfx Map Class
 * \details This class is a wrapper for a sg_map_t data structure.
 */
class GfxMap : public var::Item<sg_map_t> {
public:

	enum {
		THICKNESS_MASK = ~SG_MAP_FILL_FLAG,
		FILL = SG_MAP_FILL_FLAG
	};

	GfxMap(){}
	GfxMap(const Bitmap & bitmap, u8 o_thickness_fill = FILL | 1, s16 rotation = 0);

	void set_bitmap_center(const Bitmap & bitmap, u8 o_thickness_fill = FILL | 1, s16 rotation = 0);

	void set_dim(sg_size_t w, sg_size_t h);
	void set_shift(sg_int_t x, sg_int_t y);
	void set_dim(const Dim & dim);
	void set_shift(const Point & p);
	void set_rotation(s16 rot){ data()->rotation = rot; }
	void set_fill(){ data()->o_thickness_fill = 0; }
	void set_thickness(u8 thickness){ data()->o_thickness_fill = thickness; }
	void set_op(u8 op){ data()->op = op; }
};


/*! \brief Gfx Class
 * \details The Gfx class can be used to draw scalable graphics on bitmaps.
 *
 */
class Gfx {
public:

	/*! \details Maximum x,y value in abstract graphic space */
	static inline sg_int_t max(){ return SG_MAX; }
	/*! \details Minimum x,y value in abstract graphic space */
	static inline sg_int_t min(){ return SG_MIN; }

	/*! \details Draw the icon centered in the bitmap
	 *
	 * @param bitmap The target bitmap
	 * @param icon The icon to draw
	 * @param bounds A pointer to the bounds or null if not needed
	 */
	static void draw(Bitmap & bitmap, const sg_icon_t & icon, u8 o_thickness_fill, s16 rotation = 0, sg_bounds_t * bounds = 0);

	/*! \details Draw the icon using the specified map
	 *
	 * @param bitmap The target bitmap
	 * @param icon The icon to draw
	 * @param map The map describing how the icon will be mapped to the bitmap
	 * @param bounds A pointer to the bounds if needed (otherwise null)
	 */
	static void draw(Bitmap & bitmap, const sg_icon_t & icon, const sg_map_t & map, sg_bounds_t * bounds = 0);

	/*! \details Fill primitive
	 *
	 * @param x1 The x point to start the fill algorithm
	 * @param y1 The y point to start the fill algorithm
	 * @return
	 */
	static sg_icon_primitive_t fill(sg_int_t x1, sg_int_t y1);

	static sg_icon_primitive_t line(sg_int_t x1, sg_int_t y1, sg_int_t x2, sg_int_t y2);

	static sg_icon_primitive_t line(Point p1, Point p2){
		return line(p1.x(), p1.y(), p2.x(), p2.y());
	}

	static sg_icon_primitive_t circle(sg_int_t x, sg_int_t y, sg_size_t r);
	static sg_icon_primitive_t circle(Point p, sg_size_t r){
		return circle(p.x(), p.y(), r);
	}
	static sg_icon_primitive_t arc(sg_int_t x, sg_int_t y, sg_size_t rx, sg_size_t ry, i16 start, i16 stop, i16 rotation = 0);
	static sg_icon_primitive_t arc(Point p, sg_size_t r, i16 start, i16 stop, i16 rotation = 0){
		return arc(p.x(), p.y(), r, start, stop, rotation);
	}

	static void show(const sg_icon_primitive_t & mg);
	static void show(const sg_icon_t & icon);

	static void draw_remove(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_ok(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_bars(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_circle(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_circle_fill(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_toggle_off(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_toggle_on(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_power(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_chevron(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_arrow(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_zoom(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_reset(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_lightning(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_play(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_pause(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_button_bar(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_mic(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_clock(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_heart(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_plot(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_bike(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_mountain(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_compass(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_compass_outer(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_panel(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_panel_fill(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_sun(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_battery(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_arrowhead(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_dot(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_dash(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_message(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_contact(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_contact_fill(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_speaker(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_speaker_fill(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_antenna(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_marker(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_zoom_plus(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_zoom_minus(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_hike(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);
	static void draw_ALPHA(Bitmap & bitmap, const GfxMap & map, sg_bounds_t * bounds = 0, bool show = false);


};

};

#endif /* SGFX_GFX_HPP_ */
