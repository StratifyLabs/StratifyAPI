//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_DRAWING_HPP_
#define DRAW_DRAWING_HPP_

#include <mcu/types.h>
#include "../sgfx/Bitmap.hpp"

namespace draw {

typedef u16 drawing_size_t;
typedef i16 drawing_int_t;

typedef struct MCU_PACK {
	drawing_int_t x;
	drawing_int_t y;
} drawing_point_t;

typedef struct MCU_PACK {
	drawing_size_t w;
	drawing_size_t h;
} drawing_dim_t;


typedef struct MCU_PACK {
	sgfx::Bitmap * b;
	drawing_point_t p;
	drawing_dim_t d;
} drawing_attr_t;

typedef struct MCU_PACK {
	drawing_point_t start /*! \brief Animation start point */;
	drawing_dim_t dim /*! \brief Animate within these dimensions with start in the top left corner */;
	drawing_size_t motion_total /*! \brief Total amount of animation movement */;
} drawing_animation_attr_t;


typedef struct MCU_PACK {
	sgfx::Bitmap * b;
	sg_point_t p;
	sg_dim_t d;
} drawing_scaled_attr_t;


drawing_point_t drawing_point(drawing_int_t x, drawing_int_t y);
drawing_dim_t drawing_dim(drawing_size_t w, drawing_size_t h);

/*! \brief Drawing Attribute Class
 * \details This class contains the information needed to draw various
 * Drawing objects on a bitmap.
 *
 * This class is passed to Drawing::draw() to render graphics on a bitmap.
 *
 *
 */
class DrawingAttr{
public:

	/*! \details Construct an object */
	DrawingAttr();

	/*! \details Construct an object from a drawing_attr_t */
	DrawingAttr(const drawing_attr_t & attr);

	/*! \details Return the underlying drawing_attr_t */
	operator drawing_attr_t (){ return m_attr; }

	/*! \details Assign the bitmap point and dimensions */
	void set(sgfx::Bitmap & b, drawing_point_t p, drawing_dim_t d);

	/*! \details Set the bitmap */
	void set_bitmap(sgfx::Bitmap & b){ m_attr.b = &b; }

	/*! \details Set the dimensions.  Both width and height are from 0 to 1000. */
	void set_dim(drawing_dim_t d){ m_attr.d = d; }

	/*! \details Set the dimensions.  Both width and height are from 0 to 1000. */
	void set_dim(drawing_size_t w, drawing_size_t h){ m_attr.d.w = w; m_attr.d.h = h; }

	/*! \details Set the location.  Both x and y are from 0 to 1000. */
	void set_point(drawing_point_t p){ m_attr.p = p; }

	/*! \details Set the location.  Both x and y are from 0 to 1000. */
	void set_point(drawing_int_t x, drawing_int_t y){ m_attr.p.x = x; m_attr.p.y = y; }

	/*! \details Return the width */
	drawing_size_t w() const { return m_attr.d.w; }
	/*! \details Return the height */
	drawing_size_t h() const { return m_attr.d.h; }
	/*! \details Return the x value */
	drawing_int_t x() const { return m_attr.p.x; }
	/*! \details Return the y value */
	drawing_int_t y() const { return m_attr.p.y; }

	/*! \details Access the bitmap */
	sgfx::Bitmap & bitmap() const { return *(m_attr.b); }
	sgfx::Bitmap & b() const { return *(m_attr.b); }
	/*! \details Access the position (point) */
	drawing_point_t point() const { return m_attr.p; }
	drawing_point_t p() const { return m_attr.p; }
	/*! \details Access the dimensions (dim) */
	drawing_dim_t dim() const { return m_attr.d; }
	drawing_dim_t d() const { return m_attr.d; }

	/*! \details Access the underlying attr object */
	drawing_attr_t & attr(){ return m_attr; }

	/*! \details Calculate the scaled width (width of object on the bitmap) */
	drawing_size_t calc_w(drawing_size_t v) const;

	/*! \details Calculate the scaled height (height of object on the bitmap) */
	drawing_size_t calc_h(drawing_size_t v) const;

	/*! \details Add a drawing_point_t offset */
	DrawingAttr operator+ (drawing_point_t d) const;
	/*! \details Update the dimension (must come after adding drawing_point_t) */
	DrawingAttr operator+ (drawing_dim_t d) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square.
	 *
	 * @param v The maximum width or height
	 * @return Square dimensions
	 */
	drawing_dim_t calc_square(drawing_size_t v) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square
	 * with the given width.
	 *
	 * @param v The width (height will be calculated)
	 * @return Square dimensions
	 */
	drawing_dim_t calc_square_w(drawing_size_t v) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square
	 * with the given height.
	 *
	 * @param v The height (width will be calculated)
	 * @return Square dimensions
	 */
	drawing_dim_t calc_square_h(drawing_size_t v) const;


private:
	drawing_attr_t m_attr;
};

class DrawingScaledAttr {
public:
	operator drawing_scaled_attr_t (){ return m_attr; }

	void set(sgfx::Bitmap & b, sg_point_t p, sg_dim_t d);
	void set_bitmap(sgfx::Bitmap & b){ m_attr.b = &b; }
	void set_dim(sg_dim_t d){ m_attr.d = d; }
	void set_height(sg_size_t h){ m_attr.d.h = h; }
	void set_width(sg_size_t w){ m_attr.d.h = w; }
	void set_x(sg_int_t x){ m_attr.p.x = x; }
	void set_y(sg_int_t y){ m_attr.p.x = y; }
	void set_dim(sg_size_t w, sg_size_t h){ m_attr.d.w = w; m_attr.d.h = h; }
	void set_point(sg_point_t p){ m_attr.p = p; }

	sgfx::Bitmap * b() const { return m_attr.b; }
	sg_point_t p() const { return m_attr.p; }
	sg_dim_t d() const { return m_attr.d; }
	drawing_scaled_attr_t & attr(){ return m_attr; }

	sg_size_t w() const { return m_attr.d.w; }
	sg_size_t h() const { return m_attr.d.h; }
	sg_int_t x() const { return m_attr.p.x; }
	sg_int_t y() const { return m_attr.p.y; }

	DrawingScaledAttr operator+ (sg_point_t d) const;
	DrawingScaledAttr operator+ (sg_dim_t d) const;

	sg_size_t w(sg_size_t v) const;
	sg_size_t h(sg_size_t v) const;


private:
	drawing_scaled_attr_t m_attr;
};


/*! \brief Drawing Class
 * \details This is the base class for creating drawings.  A Drawing class
 * allows for nesting and positioning of graphics within a bitmap.
 *
 * \code
 *	void MyObject::draw(const DrawingAttr::attr){
 *		Icon my_icon;
 *		drawing_dim_t square;
 *
 *		square = attr.square_w(500);
 *
 *		//this will draw my icon centered in the bitmap specified by attr
 *		//adding drawing_point(250, 250) offset the icon by 25% of the bitmap in both x and y
 *		//adding a drawing_dim_t will then scale icon to fit in a square that is half the width of the bitmap (see square_w())
 *		//draw will call the underlying draw_to_scale() method unless Icon re-implements draw
 *		my_icon.draw(attr + drawing_point(250, 250) + square);
 *
 *	}
 *
 *
 * \endcode
 *
 */
class Drawing {
public:
	Drawing();

	sg_size_t w(sg_size_t scale, sg_dim_t d);
	sg_size_t h(sg_size_t scale, sg_dim_t d);


	/*! \details This method draws the object using the specified drawing attributes.
	 *
	 * The attributes specify which bitmap to draw on, what size to draw, and where to draw.
	 * The dimensions and position are scaled to fit on the bitmap.
	 *
	 */
	virtual void draw(const DrawingAttr & attr);
	void draw(sgfx::Bitmap & b, drawing_int_t x, drawing_int_t y, drawing_size_t w, drawing_size_t h);

	/*! \details This method will set the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void set(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

	/*! \details This method will clear the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void clear(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

	/*! \details This method will invert the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void invert(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

	/*! \details This value determines how all objects are scaled.
	 * The default value is 1000. This means a value of 500 is half the target bitmap.
	 *
	 */
	static inline drawing_size_t scale(){ return 1000; }

	/*! \brief Sets the scale value (see Element::scale() for details) */
	static inline void set_scale(drawing_size_t s){ m_scale = s; }

	/*! \details This methods draws the drawing on the specified attributes.
	 *
	 * @param attr Specifies the bitmap, point and area to draw the drawing
	 */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);
	void draw_to_scale(sgfx::Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h);


	/*! \details Return the dimensions (in pixels) if any Element is drawn on the specified bitmap */
	static sg_dim_t dim_on_bitmap(const DrawingAttr & attr);
	static sg_size_t height_on_bitmap(const DrawingAttr & attr);
	static sg_size_t width_on_bitmap(const DrawingAttr & attr);
	static sg_point_t point_on_bitmap(const DrawingAttr & attr);

	/*! \brief Returns true if element is visible */
	inline bool visible() const { return flag(FLAG_VISIBLE); }
	inline void set_visible(bool v = true){ set_flag(FLAG_VISIBLE, v); }

	inline bool align_left() const { return flag(FLAG_ALIGN_LEFT); }
	inline void set_align_left(bool v = true){ set_flag(FLAG_ALIGN_LEFT, v); }
	inline bool align_right() const { return flag(FLAG_ALIGN_RIGHT); }
	inline void set_align_right(bool v = true){ set_flag(FLAG_ALIGN_RIGHT, v); }
	inline void set_align_center(bool v = true){
		if( v ){
			set_align_right(false);
			set_align_left(false);
		}
	}

	inline bool align_top() const { return flag(FLAG_ALIGN_TOP); }
	inline void set_align_top(bool v = true){ set_flag(FLAG_ALIGN_TOP, v); }
	inline bool align_bottom() const { return flag(FLAG_ALIGN_BOTTOM); }
	inline void set_align_bottom(bool v = true){ set_flag(FLAG_ALIGN_BOTTOM, v); }
	inline void set_align_middle(bool v = true){
		if( v ){
			set_align_top(false);
			set_align_bottom(false);
		}
	}

	inline bool dark() const { return flag(FLAG_DARK); }
	inline void set_dark(bool v = true){ set_flag(FLAG_DARK, v); }

	inline bool invert() const { return flag(FLAG_INVERT); }
	inline void set_invert(bool v = true){ set_flag(FLAG_INVERT, v); }



protected:

	sg_point_t point_on_bitmap(sgfx::Bitmap & b, drawing_size_t x, drawing_size_t y, sg_dim_t d);
	sg_dim_t dim_on_bitmap(sgfx::Bitmap & b) const;

	enum {
		FLAG_VISIBLE,
		FLAG_ENABLED,
		FLAG_DRAW_CLEAR,
		FLAG_EXCLUSIVE_CHECKLIST,
		FLAG_SCROLL_VISIBLE,
		FLAG_ALIGN_LEFT,
		FLAG_ALIGN_RIGHT,
		FLAG_ALIGN_TOP,
		FLAG_ALIGN_BOTTOM,
		FLAG_BUSY,
		FLAG_CANCELLED,
		FLAG_DARK,
		FLAG_INVERT,
		FLAG_DRAWING_TOTAL
	};


	bool flag(int flag) const;
	void set_flag(int flag, bool v = true);

private:
	u32 m_flags;
	static drawing_size_t m_scale;


};

};

#endif /* DRAW_DRAWING_HPP_ */
