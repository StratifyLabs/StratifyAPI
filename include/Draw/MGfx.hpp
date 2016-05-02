/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MGFX_HPP_
#define MGFX_HPP_

#include <mcu/mcu.h>
#include "mg_types.h"

#include "Var/Item.hpp"
#include "MBitmap.hpp"
#include "MPoint.hpp"

namespace Draw {

class MGfxMap : public Var::Item<mg_map_t> {
public:
	MGfxMap(){}
	MGfxMap(const MBitmap * bitmap);

	void set_dim(mg_size_t w, mg_size_t h);
	void set_shift(mg_int_t x, mg_int_t y);
	void set_dim(const MDim & dim);
	void set_shift(const MPoint & p);
	void set_rotation(mg_size_t rot){ data()->rotation = rot; }
	void set_fill(){ data()->thickness_fill = 0; }
	void set_thickness(u8 thickness){ data()->thickness_fill = thickness; }
	void set_op(u8 op){ data()->op = op; }
};

class MGfx {
public:

	static inline mg_int_t max(){ return MG_MAX; }
	static inline mg_int_t min(){ return MG_MIN; }

	static void draw(MBitmap * bitmap, const mg_icon_t * icon, const mg_map_t & map, mg_attr_t * attr = 0);

	static mg_t fill(mg_int_t x1, mg_int_t y1);
	static mg_t line(mg_int_t x1, mg_int_t y1, mg_int_t x2, mg_int_t y2);

	static mg_t line(MPoint p1, MPoint p2){
		return line(p1.x(), p1.y(), p2.x(), p2.y());
	}

	static mg_t circle(mg_int_t x, mg_int_t y, mg_size_t r);
	static mg_t circle(MPoint p, mg_size_t r){
		return circle(p.x(), p.y(), r);
	}
	static mg_t arc(mg_int_t x, mg_int_t y, mg_size_t rx, mg_size_t ry, i16 start, i16 stop, i16 rotation = 0);
	static mg_t arc(MPoint p, mg_size_t r, i16 start, i16 stop, i16 rotation = 0){
		return arc(p.x(), p.y(), r, start, stop, rotation);
	}

	static void show(const mg_t & mg);
	static void show(const mg_icon_t * icon);

	static void draw_remove(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_ok(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_bars(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_circle(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_circle_fill(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_toggle_off(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_toggle_on(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_power(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_chevron(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_arrow(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_zoom(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_reset(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_lightning(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_play(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_pause(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_button_bar(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_mic(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_clock(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_heart(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_plot(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_bike(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_mountain(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_compass(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_compass_outer(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_panel(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_panel_fill(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_sun(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_battery(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_arrowhead(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_dot(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_dash(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_message(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_contact(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_contact_fill(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_speaker(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_speaker_fill(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_antenna(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_marker(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_zoom_plus(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_zoom_minus(MBitmap * bitmap, const MGfxMap & map, bool show = false);
	static void draw_hike(MBitmap * bitmap, const MGfxMap & map, bool show = false);



	static void draw_ALPHA(MBitmap * bitmap, const MGfxMap & map, bool show = false);


};

};

#endif /* MGFX_HPP_ */
