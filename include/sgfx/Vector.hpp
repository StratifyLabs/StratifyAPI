/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_VECTOR_HPP_
#define SGFX_VECTOR_HPP_

#include <sapi/sg_types.h>

#include "../api/WorkObject.hpp"
#include "Region.hpp"
#include "Bitmap.hpp"
#include "Pen.hpp"
#include "../var/Item.hpp"
#include "../var/Vector.hpp"
#include "../api/SgfxObject.hpp"

namespace sgfx {

/*! \brief Vecotor Map Class
 * \details This class is a wrapper for a sg_vector_map_t data structure.
 */
class VectorMap : public api::SgfxInfoObject {
public:

	/*! \details Constructs an empty vector map. */
	VectorMap(){}

	/*! \details Constructs a vector map that is centered in \a bitmap.
	 *
	 * @param bitmap Bitmap used to center map
	 * @param rotation Rotation mapping
	 *
	 * This uses method calculate_for_bitmap().
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
	void calculate_for_bitmap(const Bitmap & bitmap, s16 rotation = 0);


	void calculate_for_region(const sg_region_t & region, s16 rotation = 0);

	void set_region(const Region & region);
	void set_rotation(s16 value){ m_value.rotation = value; }

	VectorMap & operator << (const Region & a){
		set_region(a);
		return *this;
	}

	VectorMap & operator << (const s16 & a){
		m_value.rotation = a;
		return *this;
	}

	const sg_region_t & region() const { return m_value.region; }
	sg_region_t region(){ return m_value.region; }

	const sg_vector_map_t & vector_map() const { return m_value; }
	sg_vector_map_t vector_map(){ return m_value; }

	const sg_vector_map_t & map() const { return m_value; }
	sg_vector_map_t map(){ return m_value; }

	operator const sg_vector_map_t & () const { return m_value; }

private:
	sg_vector_map_t m_value;


};

class VectorPathDescription : public api::SgfxInfoObject {
public:
	VectorPathDescription(const sg_vector_path_description_t & value){ m_value = value; }
	operator const sg_vector_path_description_t & () const { return m_value; }

	enum {
		NONE = SG_VECTOR_PATH_NONE,
		MOVE = SG_VECTOR_PATH_MOVE,
		LINE = SG_VECTOR_PATH_LINE,
		QUADRATIC_BEZIER = SG_VECTOR_PATH_QUADRATIC_BEZIER,
		CUBIC_BEZIER = SG_VECTOR_PATH_CUBIC_BEZIER,
		CLOSE = SG_VECTOR_PATH_CLOSE,
		POUR = SG_VECTOR_PATH_POUR,
	};

	u16 type() const { return m_value.type; }
	sg_vector_path_move_t to_move() const { return m_value.move; }
	sg_vector_path_line_t to_line() const { return m_value.line; }
	sg_vector_path_quadtratic_bezier_t to_quadratic_bezier() const { return m_value.quadratic_bezier; }
	sg_vector_path_cubic_bezier_t to_cubic_bezier() const { return m_value.cubic_bezier; }
	sg_vector_path_pour_t to_pour() const { return m_value.pour; }

private:
	sg_vector_path_description_t m_value;
};

class VectorPath : public api::SgfxInfoObject {
public:

	VectorPath(){ memset(&m_path, 0, sizeof(m_path)); }
	VectorPath(const sg_vector_path_t & path){ m_path = path; }

	bool is_valid() const { return m_path.icon.count > 0; }

	VectorPath & operator << (const var::Vector<sg_vector_path_description_t> & vector){
		m_path.icon.list = vector.to<sg_vector_path_description_t>();
		m_path.icon.count = vector.count();
		return *this;
	}

	VectorPath & operator << (const Region & region){
		m_path.region = region.region();
		return *this;
	}

	const sg_vector_path_description_t * icon_list() const { return m_path.icon.list; }
	u32 icon_count() const { return m_path.icon.count; }

	operator const sg_vector_path_t & () const { return m_path; }
	operator sg_vector_path_t & (){ return m_path; }

	void shift(Point point);
	void scale(float value);

	VectorPath & operator += (Point point){
		shift(point);
		return *this;
	}

	VectorPath & operator *= (float value){
		scale(value);
		return *this;
	}

	const sg_vector_path_t & path() const { return m_path; }
	sg_vector_path_t & path(){ return m_path; }

private:
	sg_vector_path_t m_path;
};


/*! \brief Vector Graphics Class
 * \details The Vector class can be used to draw scalable graphics on bitmaps.
 *
 */
class Vector : public api::SgfxWorkObject {
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


	static void draw(Bitmap & bitmap, VectorPath & path, const VectorMap & map);

	//need to add arc
	static sg_vector_path_description_t get_path_move(const Point & p);
	static sg_vector_path_description_t get_path_line(const Point & p);
	static sg_vector_path_description_t get_path_pour(const Point & p);
	static sg_vector_path_description_t get_path_quadratic_bezier(const Point & control, const Point & point);
	static sg_vector_path_description_t get_path_cubic_bezier(const Point & control0, const Point & control1, const Point & point);
	static sg_vector_path_description_t get_path_close();

	static Region find_active_region(const Bitmap & bitmap);


private:
	static sg_int_t find_top(const Bitmap & bitmap);
	static sg_int_t find_bottom(const Bitmap & bitmap);
	static sg_int_t find_left(const Bitmap & bitmap);
	static sg_int_t find_right(const Bitmap & bitmap);


};

}

#endif /* SGFX_VECTOR_HPP_ */
