/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_VECTOR_HPP_
#define SGFX_VECTOR_HPP_

#include <sapi/sg_types.h>

#include "Region.hpp"
#include "Bitmap.hpp"
#include "Pen.hpp"
#include "../var/Item.hpp"

namespace sgfx {

/*! \brief Gfx Map Class
 * \details This class is a wrapper for a sg_vector_map_t data structure.
 */
class VectorMap : public var::Item<sg_vector_map_t> {
public:

	/*! \details Constructs an empty vector map. */
	VectorMap(){}

	/*! \details Constructs a vector map that is centered in \a bitmap.
	 *
	 * @param bitmap Bitmap used to center map
	 * @param rotation Rotation mapping
	 *
	 * This uses method set_bitmap_center().
	 */
	VectorMap(const Bitmap & bitmap, s16 rotation = 0);


	/*! \details Sets the map to be centered and fully occupy \a bitmap.
	 *
	 * @param bitmap The bitmap to occupy
	 * @param rotation The rotation to be applied
	 *
	 * This will size the map so that a full scale vector will completely
	 * fill the specified bitmap. This means that if the vector is
	 * rotated, parts of the vector may not fit in the bitmap.
	 *
	 */
	void set_bitmap_center(const Bitmap & bitmap, s16 rotation = 0);

	void set_region(const sg_region_t & region, s16 rotation = 0);

	void fill_region(const sg_region_t & region, s16 rotation = 0);

	void set_dim(sg_size_t w, sg_size_t h);
	void set_point(sg_int_t x, sg_int_t y);
	void set_point(const sg_point_t & p);
	void set_dim(const sg_dim_t & d);
	void set_rotation(s16 rot){ data()->rotation = rot; }

	sg_size_t width() const { return data_const()->region.dim.width; }
	sg_size_t height() const { return data_const()->region.dim.height; }
	sg_int_t x() const { return data_const()->region.point.x; }
	sg_int_t y() const { return data_const()->region.point.y; }


};


/*! \brief Vector Graphics Class
 * \details The Vector class can be used to draw scalable graphics on bitmaps.
 *
 */
class Vector {
public:

	/*! \details Maximum x,y value in abstract graphic space */
	static inline sg_int_t max(){ return SG_MAX; }
	/*! \details Minimum x,y value in abstract graphic space */
	static inline sg_int_t min(){ return SG_MIN; }

	/*! \details Draw the icon using the specified map
	 *
	 * @param bitmap The target bitmap
	 * @param icon The icon to draw
	 * @param map The map describing how the icon will be mapped to the bitmap
	 * @param bounds A pointer to the bounds if needed (otherwise null)
	 */
	static void draw(Bitmap & bitmap, const sg_vector_icon_t & icon, const sg_vector_map_t & map, sg_region_t * bounds = 0);


	static void draw_path(Bitmap & bitmap, sg_vector_path_t & path, const sg_vector_map_t & map);


	static sg_vector_primitive_t fill(const Point & p);
	static sg_vector_primitive_t fill(sg_int_t x, sg_int_t y){
		return fill(Point(x,y));
	}

	static sg_vector_primitive_t line(const Point & p1, const Point & p2);
	static sg_vector_primitive_t line(sg_int_t x1, sg_int_t y1, sg_int_t x2, sg_int_t y2){
		return line(sg_point(x1, y1), sg_point(x2, y2));
	}

	static sg_vector_primitive_t circle(const Point & p, sg_size_t r);
	static sg_vector_primitive_t circle(sg_int_t x, sg_int_t y, sg_size_t r){
		return circle(Point(x,y), r);
	}

	static sg_vector_primitive_t arc(const Point & p, sg_size_t rx, sg_size_t ry, s16 start, s16 stop, s16 rotation = 0);
	static sg_vector_primitive_t arc(sg_int_t x, sg_int_t y, sg_size_t rx, sg_size_t ry, s16 start, s16 stop, s16 rotation = 0){
		return arc(sg_point(x,y), rx, ry, start, stop, rotation);
	}

	static sg_vector_primitive_t quadratic_bezier(const Point & p1, const Point & p2, const Point & p3);
	static sg_vector_primitive_t cubic_bezier(const Point & p1, const Point & p2, const Point & p3, const Point & p4);

	static sg_vector_path_description_t get_path_move(const Point & p);
	static sg_vector_path_description_t get_path_line(const Point & p);
	static sg_vector_path_description_t get_path_pour(const Point & p);
	static sg_vector_path_description_t get_path_quadratic_bezier(const Point & control, const Point & point);
	static sg_vector_path_description_t get_path_cubic_bezier(const Point & control0, const Point & control1, const Point & point);
	static sg_vector_path_description_t get_path_close();

	static Region find_active_region(const Bitmap & bitmap);

	static void show(const sg_vector_primitive_t & mg);
	static void show(const sg_vector_icon_t & icon);

	static void draw_remove(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_ok(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_bars(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_circle(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_circle_fill(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_toggle_off(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_toggle_on(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_power(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_chevron(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_arrow(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_zoom(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_reset(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_lightning(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_play(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_pause(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_button_bar(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_mic(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_clock(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_heart(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_plot(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_bike(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_mountain(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_compass(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_compass_outer(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_panel(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_panel_fill(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_sun(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_battery(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_arrowhead(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_dot(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_dash(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_message(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_contact(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_contact_fill(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_speaker(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_speaker_fill(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_antenna(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_marker(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_zoom_plus(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_zoom_minus(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_hike(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);
	static void draw_ALPHA(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds = 0, bool show = false);

private:
	static sg_int_t find_top(const Bitmap & bitmap);
	static sg_int_t find_bottom(const Bitmap & bitmap);
	static sg_int_t find_left(const Bitmap & bitmap);
	static sg_int_t find_right(const Bitmap & bitmap);


};

}

#endif /* SGFX_VECTOR_HPP_ */
